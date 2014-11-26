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
#include "RouterStatisticsUnit.h"
#include <GlobalStatisticsUnit.h>

namespace lisnoc {

Define_Module(RouterBuffer);

void RouterBuffer::initialize(int stage)
{
    if(stage == 0) {
        LISNoCBaseModule::initialize();
        // TODO: parameter
        m_maxfill = 4;
        allowLateAck();

        m_type = par("type");
        m_portId = par("portId");
        m_vcId = par("vcId");
    } else if(stage == 1) {
        GlobalStatisticsUnit* globalSU =
                GlobalStatisticsUnit::s_getGlobalStatisticsUnit();

        m_routerSU =
                globalSU->getRouterStatisticsUnit(par("routerId"));
    }
}

void RouterBuffer::handleIncomingFlit(LISNoCFlit *msg)
{
    ASSERT(m_buffer.getLength() < m_maxfill);

    m_buffer.insert(msg);
    cancelEvent(&m_timerMsg);
    triggerSelf(1, &m_timerMsg);
}

void RouterBuffer::trySend()
{
    ASSERT(m_buffer.getLength() >= 1);

    if (canRequestTransfer((LISNoCFlit*) m_buffer.front())) {
        requestTransfer((LISNoCFlit*) m_buffer.front());
    }
}

void RouterBuffer::handleSelfMessage(cMessage *msg)
{
    trySend();
}

void RouterBuffer::doTransfer()
{
    ASSERT(m_buffer.getLength() >= 1);

    cMessage *msg = (cMessage*) m_buffer.pop();
    int delay = (simTime().inUnit(SIMTIME_NS) - msg->getArrivalTime().inUnit(SIMTIME_NS));

    m_routerSU->collectBufferLatency(m_type, m_portId, m_vcId, delay);

    sendDelayed((cMessage*) msg, SIMTIME_ZERO, "out");

    if (m_buffer.getLength() >= 1) {
        cancelEvent(&m_timerMsg);
        triggerSelf(1, &m_timerMsg);
    }
}

bool RouterBuffer::isRequestGranted(LISNoCFlowControlRequest *msg)
{
    return (m_buffer.getLength() < m_maxfill);
}

void RouterBuffer::finish() {
    cancelEvent(&m_timerMsg);
    LISNoCBaseModule::finish();
}

RouterBuffer::~RouterBuffer()
{
}

} //namespace
