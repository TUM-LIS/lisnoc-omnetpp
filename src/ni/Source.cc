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

#include "Source.h"

#include <LISNoC_m.h>

namespace lisnoc {

Define_Module(Source);

#define DELAY (simTime() + simtime_t(200,SIMTIME_NS))
#define NEXT_CYCLE (simTime() + simtime_t(2,SIMTIME_NS))

void Source::initialize()
{
    m_id = par("id");
    if (m_id == 0) {
        scheduleAt(simTime(), &m_timerMessage);
    }
}

void Source::genPacket()
{
    for (int f = 0; f < 8; f++) {
        LISNoCFlit *flit = new LISNoCFlit();
        flit->setVC(0);
        flit->setDstId(1);

        m_queue.insert(flit);
    }

    if (!m_trySendMessage.isScheduled()) {
        scheduleAt(simTime(), &m_trySendMessage);
    }
}

void Source::trySend()
{
    ASSERT(m_queue.getLength() > 0);

    m_flowControlMessage.setKind(LISNOC_REQUEST);
    send(&m_flowControlMessage, "fc_req_out", 0);

}

void Source::handleMessageGrant(LISNoCFlowControlMsg *msg)
{
    ASSERT(m_queue.getLength() > 0);

    sendDelayed((cMessage*)m_queue.pop(), SIMTIME_ZERO, "out");
}

void Source::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == &m_timerMessage) {
            genPacket();
            scheduleAt(DELAY, &m_timerMessage);
        } else if (msg == &m_trySendMessage) {
            trySend();
        }
    } else {
        handleMessageGrant((LISNoCFlowControlMsg*) msg);
    }
}

}; // namespace
