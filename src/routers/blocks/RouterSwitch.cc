//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "RouterSwitch.h"

namespace lisnoc {

Define_Module(RouterSwitch);

void RouterSwitch::initialize()
{
    m_nVCs = par("nVCs");
    m_nPorts = par("nPorts");

    m_outputArbiters.resize(m_nPorts);
    m_outputRequests.resize(m_nPorts);
    m_inputRequests.resize(m_nPorts);
    for (int p = 0; p < m_nPorts; p++) {
        m_outputArbiters[p].resize(m_nVCs);
        m_inputRequests[p].resize(m_nVCs, NULL);
        m_outputRequests[p].resize(m_nVCs, NULL);

        for (int v = 0; v < m_nVCs; v++) {
            Arbiter *arb = new Arbiter(m_nPorts, p, v);
            m_outputArbiters[p][v] = arb;
        }
    }

    m_selfSignal = new cMessage;
}

RouterSwitch::Arbiter::Arbiter(int numports, int port, int vc)
{
    m_nPorts = numports;
    m_port = port;
    m_vc = vc;

    m_transmittingWorm = false;

    m_pending = false;
    m_requests.resize(numports, false);
    m_requestsIsHead.resize(numports, false);
    m_requestsIsTail.resize(numports, false);

    m_arbitratedPort = 0;
}

void RouterSwitch::Arbiter::request(int port, int vc, bool head, bool tail)
{
    ASSERT(vc == m_vc);
    m_requests[port] = true;
    m_requestsIsHead[port] = head;
    m_requestsIsTail[port] = tail;

    m_pendingOutputReply = false;
    m_pending = true;
}

bool RouterSwitch::Arbiter::pendingArbitration()
{
    if (m_transmittingWorm) {
        return m_requests[m_arbitratedPort];
    } else {
        return m_pending;
    }
}

int RouterSwitch::Arbiter::arbitrate()
{
    ASSERT(!m_pendingOutputReply);

    m_pending = false;

    int arbp = -1;

    if(m_transmittingWorm) {
        ASSERT(m_requests[m_arbitratedPort]);
        ASSERT(m_requestsIsHead[m_arbitratedPort] == false);

        if(m_requestsIsTail[m_arbitratedPort] == true) {
            m_transmittingWorm = false;
        }

        arbp = m_arbitratedPort;

    } else {

        for (int p = 0; p < m_nPorts; p++) {
            int rrport = (p + m_arbitratedPort + 1 + m_nPorts) % m_nPorts;
            if (m_requests[rrport]) {

                ASSERT(m_requestsIsHead[rrport]);

                if(m_requestsIsTail[rrport] == false) {
                    m_arbitratedPort = rrport;
                    m_transmittingWorm = true;
                }

                arbp = rrport;

                break;
            }
        }

    }

    // Cleanup
    for (int p = 0; p < m_nPorts; p++) {
        m_requests[p] = false;
    }

    return arbp;
}

void RouterSwitch::Arbiter::setOutputReady(bool ready)
{
    m_pendingOutputReply = false;
    m_outputReady = ready;
}

bool RouterSwitch::Arbiter::getOutputReady()
{
    ASSERT(!m_pendingOutputReply || !m_pending);
    return m_outputReady;
}

void RouterSwitch::handleMessageRequest(LISNoCFlowControlRequest *request)
{
    LISNoCFlitControlInfo *controlInfo = (LISNoCFlitControlInfo*) request->getControlInfo()->dup();
    int outport = controlInfo->getOutputPort();
    int outvc = controlInfo->getVC();

    Arbiter *arb = m_outputArbiters[outport][outvc];

    int gateidx = request->getArrivalGate()->getIndex();
    int inport = gateidx / m_nVCs;
    int invc = gateidx % m_nVCs;

    arb->request(inport, invc, controlInfo->getIsHead(), controlInfo->getIsTail());
    ASSERT(!m_inputRequests[inport][invc]);
    m_inputRequests[inport][invc] = controlInfo;

    LISNoCFlowControlRequest *fwdrequest = m_outputRequests[outport][outvc];
    if (!fwdrequest) {
        fwdrequest = new LISNoCFlowControlRequest;
        fwdrequest->setControlInfo(request->getControlInfo()->dup());
        send(fwdrequest, "fc_req_out", outport*m_nVCs+outvc);
    }

    delete request;
}

void RouterSwitch::handleMessageGrant(LISNoCFlowControlGrant *grant)
{
    int gateidx = grant->getArrivalGate()->getIndex();
    int port = gateidx / m_nVCs;
    int vc = gateidx % m_nVCs;

    m_outputArbiters[port][vc]->setOutputReady(grant->getAck());

    if (m_selfSignal->isScheduled()) {
        cancelEvent(m_selfSignal);
    }

    scheduleAt(simTime(), m_selfSignal);

    delete grant;
}

void RouterSwitch::arbitrate()
{
    for (int p = 0; p < m_nPorts; p++) {
        for (int v = 0; v < m_nVCs; v++) {
            if (!m_outputArbiters[p][v]->pendingArbitration()) {
                continue;
            }

            if(!m_outputArbiters[p][v]->getOutputReady()) {
                continue;
            }

            int inport = m_outputArbiters[p][v]->arbitrate();
            ASSERT(inport >= 0);

            ASSERT(m_inputRequests[inport][v]);
            LISNoCFlowControlGrant *grant = new LISNoCFlowControlGrant;
            grant->setControlInfo(m_inputRequests[inport][v]);
            grant->setAck(true);
            sendDelayed(grant, SIMTIME_ZERO, "fc_grant_out", inport*m_nVCs+v);
            m_inputRequests[inport][v] = NULL;
        }
    }

    for (int p = 0; p < m_nPorts; p++) {
        for (int v = 0; v < m_nVCs; v++) {
            if (m_inputRequests[p][v]) {
                LISNoCFlowControlGrant *grant = new LISNoCFlowControlGrant;
                grant->setControlInfo(m_inputRequests[p][v]);
                grant->setAck(false);
                sendDelayed(grant, SIMTIME_ZERO, "fc_grant_out", p*m_nVCs+v);
                m_inputRequests[p][v] = NULL;
            }
        }
    }
}

void RouterSwitch::handleMessageFlit(LISNoCFlit *msg)
{
    LISNoCFlitControlInfo *controlInfo = (LISNoCFlitControlInfo*) msg->getControlInfo();
    int oport = controlInfo->getOutputPort();
    int ovc = controlInfo->getVC();

    send(msg, "out", oport*m_nVCs+ovc);
}

void RouterSwitch::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        arbitrate();
    } else if (msg->getKind() == LISNOC_REQUEST) {
        handleMessageRequest((LISNoCFlowControlRequest*) msg);
    } else if (msg->getKind() == LISNOC_GRANT) {
        handleMessageGrant((LISNoCFlowControlGrant*) msg);
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleMessageFlit((LISNoCFlit*) msg);
    }
}

RouterSwitch::~RouterSwitch()
{
    for (int p = 0; p < m_nPorts; p++) {
        for (int v = 0; v < m_nVCs; v++) {
            delete m_outputRequests[p][v];
        }
    }

    delete m_selfSignal;
}

} //namespace
