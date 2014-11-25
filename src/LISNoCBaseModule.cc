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

#include <LISNoCBaseModule.h>

namespace lisnoc {

LISNoCBaseModule::LISNoCBaseModule() : m_isInitialized(false)
{
}

void LISNoCBaseModule::initialize() {
    m_allowLateAck = false;
    m_flowControlMsg.setAllowLateAck(false);
    m_clock = simtime_t(2, SIMTIME_NS);

    m_pendingRequestWithLateAck.first = false;

    m_isInitialized = true;
}

void LISNoCBaseModule::allowLateAck() {
    m_allowLateAck = true;
    m_flowControlMsg.setAllowLateAck(true);
}

bool LISNoCBaseModule::canRequestTransfer(LISNoCFlit *msg)
{
    return (m_flowControlMsg.isScheduled() == false);
}

void LISNoCBaseModule::requestTransfer(LISNoCFlit *msg) {
    ASSERT(m_flowControlMsg.isScheduled() == false);

    m_flowControlMsg.setKind(LISNOC_REQUEST);
    m_flowControlMsg.setAck(false);

    if (msg) {
        m_flowControlMsg.setVC(msg->getVC());
        m_flowControlMsg.setOutputPort(msg->getOutputPort());
    }

    if (hasGate("fc_req_out", 0)) {
        send(&m_flowControlMsg, "fc_req_out", 0);
    } else {
        send(&m_flowControlMsg, "fc_req_out");
    }
}

void LISNoCBaseModule::requestTransferAfter(LISNoCFlit *msg, unsigned int numcycles) {
    ASSERT(m_isInitialized);
    ASSERT(m_flowControlMsg.isScheduled() == false);

    if (msg) {
        m_flowControlMsg.setVC(msg->getVC());
        m_flowControlMsg.setOutputPort(msg->getOutputPort());
    }

    scheduleAt(simTime() + m_clock * numcycles, &m_flowControlMsg);
}

void LISNoCBaseModule::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == &m_flowControlMsg) {
            requestTransfer(NULL);
        } else {
            handleSelfMessage(msg);
        }
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleIncomingFlit((LISNoCFlit*) msg);
    } else if (msg->getKind() == LISNOC_REQUEST) {
        handleIncomingRequest((LISNoCFlowControlMsg*) msg);
    } else if (msg->getKind() == LISNOC_GRANT) {
        handleIncomingGrant((LISNoCFlowControlMsg*) msg);
    }
}

void LISNoCBaseModule::triggerSelf(unsigned int numcycles, cMessage *msg)
{
    ASSERT(m_isInitialized);
    if (!msg) {
        msg = &m_selfTrigger;
    }

    ASSERT(!msg->isScheduled());

    simtime_t curtime = simTime();
    ASSERT((curtime.remainderForUnit(SIMTIME_NS) == 0) &&
            (curtime.inUnit(SIMTIME_NS) % 2 == 0));

    scheduleAt(curtime + m_clock * numcycles, msg);
}

void LISNoCBaseModule::handleIncomingGrant(LISNoCFlowControlMsg *msg)
{
    if (!msg->getAck()) {
        ASSERT(msg == &m_flowControlMsg); // not allowed for late ack
        requestTransferAfter(NULL, 1);
        return;
    }

    if (msg != &m_flowControlMsg) {
        // late grant
        ASSERT(m_allowLateAck);

        ASSERT(m_flowControlMsg.isScheduled());

        cancelEvent(&m_flowControlMsg);

        delete(msg);
    }

    doTransfer();
}

void LISNoCBaseModule::handleIncomingRequest(LISNoCFlowControlMsg *msg)
{
    msg->setKind(LISNOC_GRANT);
    msg->setAck(isRequestGranted(msg));

    if (!msg->getAck() && msg->getAllowLateAck()) {
        m_pendingRequestWithLateAck.first = true;
        m_pendingRequestWithLateAck.second = simTime();
    }

    if (hasGate("fc_grant_out", 0)) {
        sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out", 0);
    } else {
        sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out");
    }
}

void LISNoCBaseModule::tryLateGrant() {
    if (m_pendingRequestWithLateAck.first) {
        ASSERT(m_pendingRequestWithLateAck.second == simTime());
        LISNoCFlowControlMsg *msg = new LISNoCFlowControlMsg();
        msg->setKind(LISNOC_GRANT);
        msg->setAck(true);
        msg->setIsLateAck(true);

        if (hasGate("fc_grant_out", 0)) {
            sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out", 0);
        } else {
            sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out");
        }
    }
}


} /* namespace lisnoc */
