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

#include "RouterBuffer.h"

#define NEXT_CYCLE (simTime() + simtime_t(2, SIMTIME_NS))

namespace lisnoc {

Define_Module(RouterBuffer);

void RouterBuffer::initialize()
{
    // TODO: parameter
    m_maxfill = 4;
}

void RouterBuffer::handleIncomingFlit(LISNoCFlit *msg)
{
    ASSERT(m_buffer.getLength() < m_maxfill);

    m_buffer.insert(msg);
    scheduleAt(NEXT_CYCLE, &m_timerMsg);
}

void RouterBuffer::trySend()
{
    ASSERT(m_buffer.getLength() >= 1);

    m_flowControlMsg.setKind(LISNOC_REQUEST);

    send(&m_flowControlMsg, "fc_req_out");
}

void RouterBuffer::handleIncomingRequest(LISNoCFlowControlMsg *msg)
{
    // This is from the incoming port
    if (m_buffer.getLength() < m_maxfill) {
        msg->setKind(LISNOC_GRANT);
        sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out");
    }
}

void RouterBuffer::handleIncomingGrant(LISNoCFlowControlMsg *msg)
{
    ASSERT(m_buffer.getLength() >= 1);

    sendDelayed((cMessage*) m_buffer.pop(), SIMTIME_ZERO, "out");

    if (m_buffer.getLength() >= 1) {
         scheduleAt(NEXT_CYCLE, &m_timerMsg);
    }

}

void RouterBuffer::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        trySend();
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleIncomingFlit((LISNoCFlit*) msg);
    } else if (msg->getKind() == LISNOC_REQUEST) {
        handleIncomingRequest((LISNoCFlowControlMsg*) msg);
    } else if (msg->getKind() == LISNOC_GRANT) {
        handleIncomingGrant((LISNoCFlowControlMsg*) msg);
    }
}

} //namespace
