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
        m_outputRequests[p].resize(m_nVCs);
        m_inputRequests[p].resize(m_nVCs);

        for (int v = 0; v < m_nVCs; v++) {
            Arbiter *arb = new Arbiter(m_nPorts, p, v);
            m_outputArbiters[p][v] = arb;

            m_inputRequests[p][v] = NULL;
        }
    }
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
    return m_pending;
}

int RouterSwitch::Arbiter::arbitrate()
{
    ASSERT(!m_pendingOutputReply);

    m_pending = false;

    if(m_transmittingWorm) {
        // TODO: only if worm also requests

        ASSERT(m_requestsIsHead[m_arbitratedPort] == false);

        if(m_requestsIsTail[m_arbitratedPort] == true) {
            m_transmittingWorm = false;
        }

        return m_arbitratedPort;

    } else {

        for (int p = 0; p < m_nPorts; p++) {
            if (m_requests[p]) {

                ASSERT(m_requestsIsHead[p]);

                if(m_requestsIsTail[p] == false) {
                    m_arbitratedPort = p;
                    m_transmittingWorm = true;
                }

                return p;
            }
        }

    }

    return -1;
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

void RouterSwitch::handleMessageRequest(LISNoCFlowControlMsg *msg)
{
    int outport = msg->getOutputPort();
    int outvc = msg->getVC();

    Arbiter *arb = m_outputArbiters[outport][outvc];
    LISNoCFlowControlMsg *omsg = &m_outputRequests[outport][outvc];

    int gateidx = msg->getArrivalGate()->getIndex();
    int inport = gateidx / m_nVCs;
    int invc = gateidx % m_nVCs;

    arb->request(inport, invc, msg->getIsHead(), msg->getIsTail());
    ASSERT(m_inputRequests[inport][invc] == NULL);
    m_inputRequests[inport][invc] = msg;

    if (!omsg->isScheduled()) {
        omsg->setKind(LISNOC_REQUEST);
        send(omsg, "fc_req_out", outport*m_nVCs+outvc);
    }

}

void RouterSwitch::handleMessageGrant(LISNoCFlowControlMsg *msg)
{
    int gateidx = msg->getArrivalGate()->getIndex();
    int port = gateidx / m_nVCs;
    int vc = gateidx % m_nVCs;

    ASSERT(&m_outputRequests[port][vc] == msg);

    m_outputArbiters[port][vc]->setOutputReady(msg->getAck());

    if (m_selfSignal.isScheduled()) {
        cancelEvent(&m_selfSignal);
    }

    scheduleAt(simTime(), &m_selfSignal);
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

            LISNoCFlowControlMsg *msg = m_inputRequests[inport][v];
            ASSERT(msg);
            msg->setKind(LISNOC_GRANT);
            msg->setAck(true);
            sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out", inport*m_nVCs+v);
        }
    }

    for (int p = 0; p < m_nPorts; p++) {
        for (int v = 0; v < m_nVCs; v++) {
            LISNoCFlowControlMsg *msg = m_inputRequests[p][v];
            if (msg) {
                if (!msg->isScheduled()) {
                    msg->setKind(LISNOC_GRANT);
                    msg->setAck(false);
                    sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out", p*m_nVCs+v);
                }
                m_inputRequests[p][v] = NULL;
            }
        }
    }
}

void RouterSwitch::handleMessageFlit(LISNoCFlit *msg)
{
    int oport = msg->getOutputPort();
    int ovc = msg->getVC();

    send(msg, "out", oport*m_nVCs+ovc);
}

void RouterSwitch::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        arbitrate();
    } else if (msg->getKind() == LISNOC_REQUEST) {
        handleMessageRequest((LISNoCFlowControlMsg*) msg);
    } else if (msg->getKind() == LISNOC_GRANT) {
        handleMessageGrant((LISNoCFlowControlMsg*) msg);
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleMessageFlit((LISNoCFlit*) msg);
    }
}

} //namespace
