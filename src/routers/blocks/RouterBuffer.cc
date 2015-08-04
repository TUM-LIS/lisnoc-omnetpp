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
#include <stdio.h>

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
    ASSERT(m_buffer.getLength() < m_maxfill-1);
    //int Rout=par("routerId");
    if(strcmp("out",m_type)==0){
        m_buffer.insert(msg);
        cancelEvent(&m_timerMsg);
        triggerSelf(1, &m_timerMsg);
    }

    if(strcmp("in",m_type)==0){
        LISNoCFlitControlInfo *controlInfo = (LISNoCFlitControlInfo *) msg->getControlInfo();

        //LISNoCFlit *flit = new LISNoCFlit;
        //LISNoCFlitControlInfo *controlInfo2 = new LISNoCFlitControlInfo;
        //flit->setControlInfo(controlInfo2);

        int col = par("columns");
        int routerId = par("routerId");

        //std::cout<<"Router = "<<Rout<<" ,Data array size = "<<msg->getDataArraySize()<<std::endl;
        //    if(msg->getDataArraySize()!=0)
        //        std::cout<<" ,Data= "<<msg->getData(msg->getDataArraySize())<<std::endl;

        bool Split = controlInfo->getSplit();
        bool iX = controlInfo->getIX();
        if (iX==false){
            if (Split==true){
                m_buffer.insert(msg);
                cancelEvent(&m_timerMsg);
                triggerSelf(1, &m_timerMsg);
            }
            else{
                int dstId = controlInfo->getDstId();
                int dst2Id = controlInfo->getDst2Id();
                int ISId = controlInfo->getDst3Id();
                //int col = par("columns");
                //int routerId = par("routerId");
                bool DCNC = controlInfo->getDCNC();

                int dst1PosX = dstId%col;
                int dst2PosX = dst2Id%col;
                int routerPosX = routerId%col;

                if(((dst1PosX < routerPosX) & (dst2PosX < routerPosX)) | ((dst1PosX > routerPosX) & (dst2PosX > routerPosX))){
                    m_buffer.insert(msg);
                    cancelEvent(&m_timerMsg);
                    triggerSelf(1, &m_timerMsg);
                }
                else{
                    if(DCNC==false){
                        flit = new LISNoCFlit;
                        controlInfo2 = new LISNoCFlitControlInfo;
                        flit->setControlInfo(controlInfo2);

                        controlInfo->setSplit(true);
                        controlInfo2->setSplit(true);

                        controlInfo->setDst2Id(-1);
                        controlInfo2->setDst2Id(-1);
                        controlInfo2->setDstId(dst2Id);

                        controlInfo2->setVC(0);
                        controlInfo2->setMXaIS(false);
                        controlInfo2->setIX(false);
                        controlInfo2->setMXaDst(false);
                        controlInfo2->setDCNC(false);
                        controlInfo2->setSrcId(controlInfo->getSrcId());
                        controlInfo2->setPacketId(controlInfo->getPacketId());
                        flit->setBitLength(32);
                        flit->setByteLength(4);
                        controlInfo2->setFlitId(controlInfo->getFlitId());
                        controlInfo2->setIsHead(controlInfo->getIsHead());
                        controlInfo2->setIsTail(controlInfo->getIsTail());
                        flit->setGenerationTime(msg->getGenerationTime());
                        flit->setPacket(msg->getPacket());

                        m_buffer.insert(msg);

                        flit->setDataArraySize(msg->getDataArraySize());

                        for(unsigned int d = 0; d < msg->getDataArraySize(); d++){
                            flit->setData(d,msg->getData(d));
                        }

                        m_buffer.insert(flit);
                        cancelEvent(&m_timerMsg);
                        triggerSelf(1, &m_timerMsg);
                    }
                    else{
                        flit = new LISNoCFlit;
                        controlInfo2 = new LISNoCFlitControlInfo;
                        flit->setControlInfo(controlInfo2);

                        controlInfo->setSplit(true);
                        controlInfo2->setSplit(true);

                        controlInfo->setDstId(dstId);
                        controlInfo->setDst2Id(-1);
                        controlInfo->setDst3Id(-1);

                        controlInfo->setMXaIS(false);
                        controlInfo->setIX(false);
                        controlInfo->setMXaDst(true);

                        controlInfo2->setDstId(ISId);
                        controlInfo2->setDst2Id(dstId);
                        controlInfo2->setDst3Id(dst2Id);

                        controlInfo2->setMXaIS(true);
                        controlInfo2->setIX(false);
                        controlInfo2->setMXaDst(true);




                        controlInfo2->setVC(0);
                        controlInfo2->setDCNC(true);
                        controlInfo2->setSrcId(controlInfo->getSrcId());
                        controlInfo2->setPacketId(controlInfo->getPacketId());
                        flit->setBitLength(32);
                        flit->setByteLength(4);
                        controlInfo2->setFlitId(controlInfo->getFlitId());
                        controlInfo2->setIsHead(controlInfo->getIsHead());
                        controlInfo2->setIsTail(controlInfo->getIsTail());
                        controlInfo2->setSrc2Id(controlInfo->getSrc2Id());
                        flit->setGenerationTime(msg->getGenerationTime());
                        flit->setPacket(msg->getPacket());



                        m_buffer.insert(msg);

                        flit->setDataArraySize(msg->getDataArraySize());

                        for(unsigned int d = 0; d < msg->getDataArraySize(); d++){
                            flit->setData(d,msg->getData(d));
                        }
                        m_buffer.insert(flit);
                        cancelEvent(&m_timerMsg);
                        triggerSelf(1, &m_timerMsg);
                    }
                }
            }
        }
        else{
            int dstId = controlInfo->getDstId();
            if (dstId!=routerId){
                m_buffer.insert(msg);
                cancelEvent(&m_timerMsg);
                triggerSelf(1, &m_timerMsg);
            }
            else{
                flit = new LISNoCFlit;
                controlInfo2 = new LISNoCFlitControlInfo;
                flit->setControlInfo(controlInfo2);

                controlInfo2->setSplit(true);

                controlInfo2->setVC(0);
                controlInfo2->setMXaIS(true);
                controlInfo2->setIX(false);
                controlInfo->setIX(false);
                controlInfo2->setMXaDst(true);
                controlInfo2->setDCNC(true);
                controlInfo2->setSrcId(controlInfo->getSrcId());
                controlInfo2->setPacketId(controlInfo->getPacketId());
                flit->setBitLength(32);
                flit->setByteLength(4);
                controlInfo2->setFlitId(controlInfo->getFlitId());
                controlInfo2->setIsHead(controlInfo->getIsHead());
                controlInfo2->setIsTail(controlInfo->getIsTail());
                flit->setGenerationTime(msg->getGenerationTime());
                flit->setPacket(msg->getPacket());



                flit->setDataArraySize(msg->getDataArraySize());

                for(unsigned int d = 0; d < msg->getDataArraySize(); d++){
                    flit->setData(d,msg->getData(d));
                }


                controlInfo->setDstId(controlInfo->getDst2Id());
                controlInfo2->setDstId(controlInfo->getDst3Id());
                controlInfo->setDst2Id(-1);
                controlInfo->setDst3Id(-1);
                controlInfo2->setDst2Id(-1);
                controlInfo2->setDst3Id(-1);
                m_buffer.insert(msg);
                m_buffer.insert(flit);
                cancelEvent(&m_timerMsg);
                triggerSelf(1, &m_timerMsg);
            }
        }
    }
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
    return (m_buffer.getLength() < m_maxfill-1);
}

void RouterBuffer::finish() {
    cancelEvent(&m_timerMsg);
    LISNoCBaseModule::finish();
}

RouterBuffer::~RouterBuffer()
{
}

} //namespace
