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

void Source::initialize()
{
    m_id = par("id");
    if (m_id == 0) {
        triggerSelf(0, &m_timerMessage);
    }
}

void Source::genPacket()
{
    for (int f = 0; f < 3; f++) {
        LISNoCFlit *flit = new LISNoCFlit();
        flit->setVC(0);
        flit->setDstId(15);

        m_queue.insert(flit);
    }

    requestTransfer((LISNoCFlit*) m_queue.front());
}

void Source::doTransfer()
{
    ASSERT(m_queue.getLength() > 0);

    sendDelayed((cMessage*)m_queue.pop(), SIMTIME_ZERO, "out", 0);

    if (m_queue.getLength() > 0) {
        requestTransferAfter((LISNoCFlit*) m_queue.front(), 1);
    }

}

void Source::handleSelfMessage(cMessage *msg) {
    ASSERT(msg == &m_timerMessage);

    genPacket();
//    triggerSelf(100, &m_timerMessage);
}

}; // namespace
