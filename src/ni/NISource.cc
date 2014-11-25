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

#include "NISource.h"

#include <LISNoC_m.h>

namespace lisnoc {

Define_Module(NISource);

void NISource::initialize()
{
    LISNoCBaseModule::initialize();

    m_timerMessage = new cMessage;

    m_id = par("id");
    if ((int) par("dstId") >= 0) {
        triggerSelf((int) par("initialdelay"), m_timerMessage);
    }
}

void NISource::genPacket()
{
    int numflits = 8;
    int dstId = par("dstId");

    for (int f = 0; f < numflits; f++) {
        LISNoCFlit *flit = new LISNoCFlit();
        flit->setVC(0);
        flit->setDstId(dstId);
        flit->setBitLength(32);
        flit->setByteLength(4);
        flit->setFlitId(f);
        flit->setIsHead(f==0);
        flit->setIsTail(f==numflits-1);
        flit->setGenerationTime(simTime());

        m_queue.insert(flit);
    }

    if (canRequestTransfer((LISNoCFlit*) m_queue.front())) {
        requestTransfer((LISNoCFlit*) m_queue.front());
    }
}

void NISource::doTransfer()
{
    ASSERT(m_queue.getLength() > 0);

    ((LISNoCFlit*) m_queue.front())->setSendTime(simTime());

    sendDelayed((cMessage*)m_queue.pop(), SIMTIME_ZERO, "out", 0);

    if (m_queue.getLength() > 0) {
        requestTransferAfter((LISNoCFlit*) m_queue.front(), 1);
    }

}

void NISource::handleSelfMessage(cMessage *msg) {
    ASSERT(msg == m_timerMessage);

    genPacket();
    triggerSelf((int) par("genDelay"), m_timerMessage);
}

NISource::~NISource() {

    while (!m_queue.empty()) {
        LISNoCFlit *flit = (LISNoCFlit*) m_queue.pop();
        delete flit;
    }
    cancelAndDelete(m_timerMessage);
}

}; // namespace
