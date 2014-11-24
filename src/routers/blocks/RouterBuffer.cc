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

    allowLateAck();
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

    requestTransfer();
}

void RouterBuffer::handleSelfMessage(cMessage *msg)
{
    trySend();
}

void RouterBuffer::doTransfer()
{
    ASSERT(m_buffer.getLength() >= 1);

    sendDelayed((cMessage*) m_buffer.pop(), SIMTIME_ZERO, "out");

    if (m_buffer.getLength() >= 1) {
        triggerSelf(1);
    }
}

bool RouterBuffer::isRequestGranted(LISNoCFlowControlMsg *msg)
{
    return (m_buffer.getLength() < m_maxfill);
}


} //namespace
