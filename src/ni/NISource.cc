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
#include <random>
#include <math.h>

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
    int dst2Id = par("dst2Id");
    int src2Id = par("src2Id");
    int col = par("columns");
    bool DCNC = par("DCNC");

    int src1PosX = m_id%col;
    int src1PosY = m_id/col;

    int src2PosX = src2Id%col;
    int src2PosY = src2Id/col;

    //int dst1PosX = dstId%col;
    //int dst2PosX = dst2Id%col;

    int ISPosX = floor((src1PosX + src2PosX)/2);
    int ISPosY = floor((src1PosY + src2PosY)/2);

    int ISId = (col*ISPosY)+ISPosX;

        LISNoCPacket *packet = new LISNoCPacket(m_nextPacketId);

        for (int f = 0; f < numflits; f++) {
            LISNoCFlit *flit = new LISNoCFlit;
            LISNoCFlitControlInfo *controlInfo = new LISNoCFlitControlInfo;
            flit->setControlInfo(controlInfo);
            controlInfo->setVC(0);
            controlInfo->setDstId(dstId);
            controlInfo->setDst2Id(dst2Id);
            controlInfo->setDst3Id(ISId);
            controlInfo->setSrc2Id(src2Id);
            controlInfo->setMXaIS(false);
            controlInfo->setIX(false);
            controlInfo->setMXaDst(false);
            if(DCNC==false){
                controlInfo->setDCNC(false);
            }
            else{
                controlInfo->setDCNC(true);
            }
            controlInfo->setSplit(false);
            controlInfo->setSrcId(m_id);
            controlInfo->setPacketId(m_nextPacketId);
            flit->setBitLength(32);
            flit->setByteLength(4);

            flit->setDataArraySize(4);

            for(int d = 0; d < 4; d++){
                uint8_t x = rand()%256;
                flit->setData(d,x);
            }
            controlInfo->setFlitId(f);
            controlInfo->setIsHead(f==0);
            controlInfo->setIsTail(f==numflits-1);
            flit->setGenerationTime(simTime());
            flit->setPacket(packet);

            m_queue.insert(flit);

            int flitsize = flit->getDataArraySize();
            std::cout<<"Source = "<<m_id<<" ,Data array size = "<<flitsize<<std::endl;
            if(flitsize!=0)
            {
                for(int i = 0; i < flitsize; i++){
                    std::cout<<" ,Data= "<<unsigned(flit->getData(i))<<std::endl;
                }

            }

            //std::cout<<"XOR of 1 and 2 = "<<unsigned(flit->getData(0)^flit->getData(1))<<std::endl;


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
