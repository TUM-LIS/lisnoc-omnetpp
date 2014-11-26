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

void LISNoCBaseModule::initialize() {
    m_allowLateAck = false;
    m_clock = simtime_t(1, SIMTIME_NS);

    m_pendingRequestWithLateAck.first = NULL;

    m_isInitialized = true;

    m_activeRequest = false;

    m_selfTrigger = new cMessage;
    m_delayedTransfer = NULL;
}

void LISNoCBaseModule::allowLateAck() {
    m_allowLateAck = true;
}

bool LISNoCBaseModule::canRequestTransfer(LISNoCFlit *msg)
{
    return !m_activeRequest;
}

LISNoCFlowControlRequest *LISNoCBaseModule::createFlowControlRequest(LISNoCFlit *flit)
{
    ASSERT(flit);
    LISNoCFlowControlRequest *msg = new LISNoCFlowControlRequest;
    msg->setAllowLateAck(m_allowLateAck);

    if (flit) {
        msg->setControlInfo(flit->getControlInfo()->dup());
    }

    return msg;
}

LISNoCFlowControlGrant *LISNoCBaseModule::createFlowControlGrant(LISNoCFlowControlRequest *request)
{
    ASSERT(request);
    LISNoCFlowControlGrant *msg = new LISNoCFlowControlGrant;

    if (request) {
        msg->setControlInfo(request->getControlInfo()->dup());
    }

    return msg;
}

LISNoCFlowControlRequest *LISNoCBaseModule::createFlowControlRequest(LISNoCFlowControlGrant *grant)
{
    ASSERT(grant);
    LISNoCFlowControlRequest *msg = new LISNoCFlowControlRequest;
    msg->setAllowLateAck(m_allowLateAck);

    if (grant) {
        msg->setControlInfo(grant->getControlInfo()->dup());
    }

    return msg;
}

void LISNoCBaseModule::requestTransfer(LISNoCFlit *actualMsg) {
    ASSERT(canRequestTransfer(actualMsg));

    LISNoCFlowControlRequest *msg = createFlowControlRequest(actualMsg);

    if (hasGate("fc_req_out", 0)) {
        send(msg, "fc_req_out", 0);
    } else {
        send(msg, "fc_req_out");
    }

    m_activeRequest = true;
}

void LISNoCBaseModule::delayedTransfer() {
    ASSERT(m_delayedTransfer);

    if (hasGate("fc_req_out", 0)) {
        send(m_delayedTransfer, "fc_req_out", 0);
    } else {
        send(m_delayedTransfer, "fc_req_out");
    }

    m_delayedTransfer = NULL;
}

void LISNoCBaseModule::requestTransferAfter(LISNoCFlit *actualMsg, unsigned int numcycles) {
    ASSERT(m_isInitialized);
    ASSERT(canRequestTransfer(actualMsg));
    ASSERT(!m_delayedTransfer);

    m_delayedTransfer = createFlowControlRequest(actualMsg);

    scheduleAt(simTime() + m_clock * numcycles, m_delayedTransfer);

    m_activeRequest = true;
}

void LISNoCBaseModule::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == m_delayedTransfer) {
            delayedTransfer();
        } else {
            handleSelfMessage(msg);
        }
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleIncomingFlit((LISNoCFlit*) msg);
    } else if (msg->getKind() == LISNOC_REQUEST) {
        handleIncomingRequest((LISNoCFlowControlRequest*) msg);
    } else if (msg->getKind() == LISNOC_GRANT) {
        handleIncomingGrant((LISNoCFlowControlGrant*) msg);
    }
}

void LISNoCBaseModule::triggerSelf(unsigned int numcycles, cMessage *msg)
{
    ASSERT(m_isInitialized);
    if (!msg) {
        msg = m_selfTrigger;
    }

    ASSERT(!msg->isScheduled());

    simtime_t curtime = simTime();
    ASSERT((curtime.remainderForUnit(SIMTIME_NS) == 0));

    scheduleAt(curtime + m_clock * numcycles, msg);
}

void LISNoCBaseModule::handleIncomingGrant(LISNoCFlowControlGrant *grant)
{
    if (!grant->getAck()) {
        ASSERT(!m_delayedTransfer);
        m_delayedTransfer = createFlowControlRequest(grant);
        scheduleAt(simTime() + m_clock, m_delayedTransfer);

        delete grant;
        return;
    }

    if (grant->getIsLateAck()) {
        // late grant
        ASSERT(m_allowLateAck);

        ASSERT(m_delayedTransfer->isScheduled());

        cancelEvent(m_delayedTransfer);

        delete m_delayedTransfer;
        m_delayedTransfer = NULL;
    }

    m_activeRequest = false;
    delete grant;

    doTransfer();
}

void LISNoCBaseModule::handleIncomingRequest(LISNoCFlowControlRequest *request)
{
    bool ack = isRequestGranted(request);

    delete m_pendingRequestWithLateAck.first;
    m_pendingRequestWithLateAck.first = NULL;

    if (!ack && request->getAllowLateAck()) {
        m_pendingRequestWithLateAck.first = request;
        m_pendingRequestWithLateAck.second = simTime();
    }

    LISNoCFlowControlGrant *grant = createFlowControlGrant(request);
    grant->setAck(ack);

    if (hasGate("fc_grant_out", 0)) {
        sendDelayed(grant, SIMTIME_ZERO, "fc_grant_out", 0);
    } else {
        sendDelayed(grant, SIMTIME_ZERO, "fc_grant_out");
    }

    if (!(!ack && request->getAllowLateAck())) {
        delete request;
    }
}

void LISNoCBaseModule::tryLateGrant() {
    if (m_pendingRequestWithLateAck.first) {
        ASSERT(m_pendingRequestWithLateAck.second == simTime());
        LISNoCFlowControlRequest *request = m_pendingRequestWithLateAck.first;
        LISNoCFlowControlGrant *grant = createFlowControlGrant(request);
        grant->setAck(true);
        grant->setIsLateAck(true);

        if (hasGate("fc_grant_out", 0)) {
            sendDelayed(grant, SIMTIME_ZERO, "fc_grant_out", 0);
        } else {
            sendDelayed(grant, SIMTIME_ZERO, "fc_grant_out");
        }

        delete m_pendingRequestWithLateAck.first;
        m_pendingRequestWithLateAck.first = NULL;
    }
}

void LISNoCBaseModule::finish()
{
    cancelEvent(m_selfTrigger);
    if (m_delayedTransfer) {
        cancelEvent(m_delayedTransfer);
    }
}

LISNoCBaseModule::~LISNoCBaseModule()
{
    delete m_selfTrigger;
    delete m_delayedTransfer;
    delete m_pendingRequestWithLateAck.first;
}

} /* namespace lisnoc */
