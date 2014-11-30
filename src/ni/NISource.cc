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

    m_abortAccessLatency = par("abortAccessLatency");

    m_nextPacketId = 0;
}

void NISource::genPacket()
{
    int numflits = par("flitsPerPacket");
    int dstId = par("dstId");

    LISNoCPacket *packet = new LISNoCPacket(m_nextPacketId);

    for (int f = 0; f < numflits; f++) {
        LISNoCFlit *flit = new LISNoCFlit;
        LISNoCFlitControlInfo *controlInfo = new LISNoCFlitControlInfo;
        flit->setControlInfo(controlInfo);
        controlInfo->setVC(0);
        controlInfo->setDstId(dstId);
        controlInfo->setSrcId(m_id);
        controlInfo->setPacketId(m_nextPacketId);
        flit->setBitLength(32);
        flit->setByteLength(4);
        controlInfo->setFlitId(f);
        controlInfo->setIsHead(f==0);
        controlInfo->setIsTail(f==numflits-1);
        flit->setGenerationTime(simTime());
        flit->setPacket(packet);

        m_queue.insert(flit);
    }

    m_nextPacketId++;

    if (canRequestTransfer((LISNoCFlit*) m_queue.front())) {
        requestTransfer((LISNoCFlit*) m_queue.front());
    }
}

void NISource::doTransfer()
{
    ASSERT(m_queue.getLength() > 0);

    // Pop next flit from queue
    LISNoCFlit *flit = (LISNoCFlit*) m_queue.pop();

    if ((simTime() - flit->getGenerationTime()).inUnit(SIMTIME_NS) >= m_abortAccessLatency) {
        endSimulation();
    }
    flit->setSendTime(simTime());

    sendDelayed(flit, SIMTIME_ZERO, "out", 0);

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
    cancelAndDelete(m_timerMessage);
}

}; // namespace
