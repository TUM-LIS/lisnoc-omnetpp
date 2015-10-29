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
    bool UC = par("UC");
    int Algo = par("Algo");
    bool NC_better_check = par("NC_better_check");

    int src1PosX = m_id%col;
    int src1PosY = m_id/col;

    int src2PosX = src2Id%col;
    int src2PosY = src2Id/col;

    int dst1PosX = dstId%col;
    int dst1PosY = dstId/col;

    int dst2PosX = dst2Id%col;
    int dst2PosY = dst2Id/col;

    int ISPosX;
    int ISPosY;
    int ISId;
    int IDPosX;
    int IDPosY;
    int IDId;


    if(DCNC==true){
    switch(Algo){

    case 1:  //MP Algorithm
    {
        ISPosX = floor((src1PosX + src2PosX)/2);
        ISPosY = floor((src1PosY + src2PosY)/2);
        IDPosX = floor((dst1PosX + dst2PosX)/2);
        IDPosY = floor((dst1PosY + dst2PosY)/2);
        break;
    }
    case 2:  //RS Algorithm
    {
        srand(m_id+src2Id);
        ISPosX = (rand()%(abs(src1PosX-src2PosX)+1))+(int)fmin(src1PosX,src2PosX);
        ISPosY = (rand()%(abs(src1PosY-src2PosY)+1))+(int)fmin(src1PosY,src2PosY);
        IDPosX = (rand()%(abs(dst1PosX-dst2PosX)+1))+(int)fmin(dst1PosX,dst2PosX);
        IDPosY = (rand()%(abs(dst1PosY-dst2PosY)+1))+(int)fmin(dst1PosY,dst2PosY);
        break;
    }
    case 3:  //SD Algorithm
    {
        int Distance_S1D1 = abs(src1PosX - dst1PosX) + abs(src1PosY - dst1PosY);    //1
        int Distance_S1D2 = abs(src1PosX - dst2PosX) + abs(src1PosY - dst2PosY);    //2
        int Distance_S2D1 = abs(src2PosX - dst1PosX) + abs(src2PosY - dst1PosY);    //3
        int Distance_S2D2 = abs(src2PosX - dst2PosX) + abs(src2PosY - dst2PosY);    //4
        int min_value;

        min_value = Distance_S1D1;
        if (Distance_S1D2 < min_value)
            min_value = Distance_S1D2;
        if (Distance_S2D1 < min_value)
            min_value = Distance_S2D1;
        if (Distance_S2D2 < min_value)
            min_value = Distance_S2D2;

        if(min_value == Distance_S1D1)
        {
            ISPosX = src1PosX;
            ISPosY = src1PosY;
            IDPosX = dst1PosX;
            IDPosY = dst1PosY;
        }else if (min_value == Distance_S1D2){
            ISPosX = src1PosX;
            ISPosY = src1PosY;
            IDPosX = dst2PosX;
            IDPosY = dst2PosY;
        }else if (min_value == Distance_S2D1){
            ISPosX = src2PosX;
            ISPosY = src2PosY;
            IDPosX = dst1PosX;
            IDPosY = dst1PosY;
        }else{
            ISPosX = src2PosX;
            ISPosY = src2PosY;
            IDPosX = dst2PosX;
            IDPosY = dst2PosY;
        }
        break;
    }
    case 4:  //MSD Algorithm
    {
        int MP_S_PosX = floor((src1PosX + src2PosX)/2);
        int MP_S_PosY = floor((src1PosY + src2PosY)/2);
        int MP_D_PosX = floor((dst1PosX + dst2PosX)/2);
        int MP_D_PosY = floor((dst1PosY + dst2PosY)/2);

        int SD_Distance_S1D1 = abs(src1PosX - dst1PosX) + abs(src1PosY - dst1PosY);    //1
        int SD_Distance_S1D2 = abs(src1PosX - dst2PosX) + abs(src1PosY - dst2PosY);    //2
        int SD_Distance_S2D1 = abs(src2PosX - dst1PosX) + abs(src2PosY - dst1PosY);    //3
        int SD_Distance_S2D2 = abs(src2PosX - dst2PosX) + abs(src2PosY - dst2PosY);    //4
        int SD_min_value;

        SD_min_value = SD_Distance_S1D1;
        if (SD_Distance_S1D2 < SD_min_value)
            SD_min_value = SD_Distance_S1D2;
        if (SD_Distance_S2D1 < SD_min_value)
            SD_min_value = SD_Distance_S2D1;
        if (SD_Distance_S2D2 < SD_min_value)
            SD_min_value = SD_Distance_S2D2;

        int SD_S_PosX;
        int SD_S_PosY;
        int SD_D_PosX;
        int SD_D_PosY;

        if(SD_min_value == SD_Distance_S1D1)
        {
            SD_S_PosX = src1PosX;
            SD_S_PosY = src1PosY;
            SD_D_PosX = dst1PosX;
            SD_D_PosY = dst1PosY;
        }else if (SD_min_value == SD_Distance_S1D2){
            SD_S_PosX = src1PosX;
            SD_S_PosY = src1PosY;
            SD_D_PosX = dst2PosX;
            SD_D_PosY = dst2PosY;
        }else if (SD_min_value == SD_Distance_S2D1){
            SD_S_PosX = src2PosX;
            SD_S_PosY = src2PosY;
            SD_D_PosX = dst1PosX;
            SD_D_PosY = dst1PosY;
        }else{
            SD_S_PosX = src2PosX;
            SD_S_PosY = src2PosY;
            SD_D_PosX = dst2PosX;
            SD_D_PosY = dst2PosY;
        }

        int Distance_MP_S_MP_D = abs(MP_S_PosX - MP_D_PosX) + abs(MP_S_PosY - MP_D_PosY);    //MP Src and MP Dest
        int Distance_MP_S_SD_D = abs(MP_S_PosX - SD_D_PosX) + abs(MP_S_PosY - SD_D_PosY);    //MP Src and SD Dest
        int Distance_SD_S_MP_D = abs(SD_S_PosX - MP_D_PosX) + abs(SD_S_PosY - MP_D_PosY);    //SD Src and MP Dest
        int Distance_SD_S_SD_D = abs(SD_S_PosX - SD_D_PosX) + abs(SD_S_PosY - SD_D_PosY);    //SD Src and SD Dest
        int MSD_min_value;

        MSD_min_value = Distance_MP_S_MP_D;
        if (Distance_MP_S_SD_D < MSD_min_value)
            MSD_min_value = Distance_MP_S_SD_D;
        if (Distance_SD_S_MP_D < MSD_min_value)
            MSD_min_value = Distance_SD_S_MP_D;
        if (Distance_SD_S_SD_D < MSD_min_value)
            MSD_min_value = Distance_SD_S_SD_D;

        if(MSD_min_value == Distance_MP_S_MP_D)
        {
            ISPosX = MP_S_PosX;
            ISPosY = MP_S_PosY;
            IDPosX = MP_D_PosX;
            IDPosY = MP_D_PosY;
        }else if (MSD_min_value == Distance_MP_S_SD_D){
            ISPosX = MP_S_PosX;
            ISPosY = MP_S_PosY;
            IDPosX = SD_D_PosX;
            IDPosY = SD_D_PosY;
        }else if (MSD_min_value == Distance_SD_S_MP_D){
            ISPosX = SD_S_PosX;
            ISPosY = SD_S_PosY;
            IDPosX = MP_D_PosX;
            IDPosY = MP_D_PosY;
        }else{
            ISPosX = SD_S_PosX;
            ISPosY = SD_S_PosY;
            IDPosX = SD_D_PosX;
            IDPosY = SD_D_PosY;
        }
        break;
    }
    }

        ISId = (col*ISPosY)+ISPosX;
        IDId = (col*IDPosY)+IDPosX;


    }

    int dest1=dstId;
    int dest2=dst2Id;

    if(DCNC == true){
        int S1D1 = abs(src1PosX - dst1PosX) + abs(src1PosY - dst1PosY);
        int S1D2 = abs(src1PosX - dst2PosX) + abs(src1PosY - dst2PosY);
        int S2D1 = abs(src2PosX - dst1PosX) + abs(src2PosY - dst1PosY);
        int S2D2 = abs(src2PosX - dst2PosX) + abs(src2PosY - dst2PosY);


        if((S1D1 < S1D2) && (S2D1 > S2D2)){
            dest1 = dstId;
            dest2 = dst2Id;
        }else if((S1D1 > S1D2) && (S2D1 < S2D2)){
            dest1 = dst2Id;
            dest2 = dstId;
        }else if((S1D1 < S1D2) && (S2D1 < S2D2)){
            if(S1D2 > S2D2){
                dest1 = dstId;
                dest2 = dst2Id;
            }else if(S1D2 < S2D2){
                dest1 = dst2Id;
                dest2 = dstId;
            }else if(S1D2 == S2D2){
                if(S1D1 < S2D1){
                    dest1 = dstId;
                    dest2 = dst2Id;
                }else if(S1D1 > S2D1){
                    dest1 = dst2Id;
                    dest2 = dstId;
                }else if(S1D1 == S2D1){
                    if(m_id < src2Id){
                        if(dstId < dst2Id){
                            dest1 = dstId;
                            dest2 = dst2Id;
                        }
                        else{
                            dest1 = dst2Id;
                            dest2 = dstId;
                        }
                    }
                    else{
                        if(dstId < dst2Id){
                            dest1 = dst2Id;
                            dest2 = dstId;
                        }
                        else{
                            dest1 = dstId;
                            dest2 = dst2Id;
                        }

                    }
                }
            }
        }else if((S1D1 > S1D2) && (S2D1 > S2D2)){
            if(S1D1 > S2D1){
                dest1 = dst2Id;
                dest2 = dstId;
            }else if(S1D1 < S2D1){
                dest1 = dstId;
                dest2 = dst2Id;
            }else if(S1D1 == S2D1){
                if(S1D2 < S2D2){
                    dest1 = dst2Id;
                    dest2 = dstId;
                }else if(S1D2 > S2D2){
                    dest1 = dstId;
                    dest2 = dst2Id;
                }else if(S1D2 == S2D2){
                    if(m_id < src2Id){
                        if(dst2Id < dstId){
                            dest1 = dst2Id;
                            dest2 = dstId;
                        }
                        else{
                            dest1 = dstId;
                            dest2 = dst2Id;
                        }
                    }
                    else{
                        if(dst2Id < dstId){
                            dest1 = dstId;
                            dest2 = dst2Id;
                        }
                        else{
                            dest1 = dst2Id;
                            dest2 = dstId;
                        }

                    }
                }
            }
        }else if((S1D1 == S1D2) && (S2D1 != S2D2)){
            if (S2D1 < S2D2){
                dest1 = dst2Id;
                dest2 = dstId;
            }else{
                dest1 = dstId;
                dest2 = dst2Id;
            }
        }else if((S1D1 != S1D2) && (S2D1 == S2D2)){
            if (S1D1 < S1D2){
                dest1 = dstId;
                dest2 = dst2Id;
            }else{
                dest1 = dst2Id;
                dest2 = dstId;
            }
        }else if((S1D1 == S1D2) && (S2D1 == S2D2)){
            if(m_id < src2Id){
                if(dstId < dst2Id){
                    dest1 = dstId;
                    dest2 = dst2Id;
                }
                else{
                    dest1 = dst2Id;
                    dest2 = dstId;
                }
            }
            else{
                if(dstId < dst2Id){
                    dest1 = dst2Id;
                    dest2 = dstId;
                }
                else{
                    dest1 = dstId;
                    dest2 = dst2Id;
                }

            }
        }

        if(NC_better_check == true){
            int dest1PosX = dest1%col;
            int dest1PosY = dest1/col;
            int dest2PosX = dest2%col;
            int dest2PosY = dest2/col;

            int hops_MC = abs(src1PosX - dest2PosX) + abs(src1PosY - dest2PosY) + abs(src2PosX - dest1PosX) + abs(src2PosY - dest1PosY);
            int hops_NC = abs(src1PosX - ISPosX) + abs(src1PosY - ISPosY) + abs(src2PosX - ISPosX) + abs(src2PosY - ISPosY) + abs(ISPosX - IDPosX) + abs(ISPosY - IDPosY) + abs(IDPosX - dest1PosX) + abs(IDPosY - dest1PosY) + abs(IDPosX - dest2PosX) + abs(IDPosY - dest2PosY);
            if(hops_MC < hops_NC){
                DCNC = false;
            }
        }
    }

        LISNoCPacket *packet = new LISNoCPacket(m_nextPacketId);

        if(UC==false){
            for (int f = 0; f < numflits; f++) {
                LISNoCFlit *flit = new LISNoCFlit;
                LISNoCFlitControlInfo *controlInfo = new LISNoCFlitControlInfo;
                flit->setControlInfo(controlInfo);
                controlInfo->setVC(0);
                controlInfo->setDstId(dest1);
                controlInfo->setDst2Id(dest2);
                controlInfo->setDst3Id(-1);
                controlInfo->setSrc2Id(src2Id);
                controlInfo->setMXaIS(false);
                controlInfo->setIX(false);
                controlInfo->setMXaDst(false);
                if(DCNC==false){
                    controlInfo->setDCNC(false);
                }
                else{
                    controlInfo->setDCNC(true);
                    controlInfo->setDst3Id(ISId);
                    controlInfo->setDst4Id_ID(IDId);
                }
                controlInfo->setUC(false);
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
                controlInfo->setHopCount(-1);
                //std::cout<<"SOURCE, Source = "<<m_id<<", Flit ID: "<<f<<", Generation Time: "<<flit->getGenerationTime()<<endl;
                //std::cout<<"SOURCE, Source = "<<m_id<<", Packet ID: "<<controlInfo->getPacketId()<<", Flit ID: "<<f<<", DATA: "<<unsigned(flit->getData(0))<<", "<<unsigned(flit->getData(1))<<", "<<unsigned(flit->getData(2))<<", "<<unsigned(flit->getData(3))<<endl;
                flit->setPacket(packet);

                m_queue.insert(flit);


            }


            m_nextPacketId++;
        }
        else{
            for (int f = 0; f < numflits; f++) {
                LISNoCFlit *flit = new LISNoCFlit;
                LISNoCFlitControlInfo *controlInfo = new LISNoCFlitControlInfo;
                flit->setControlInfo(controlInfo);
                controlInfo->setVC(0);
                controlInfo->setDstId(dstId);
                controlInfo->setDst2Id(-1);
                controlInfo->setDst3Id(-1);
                controlInfo->setMXaIS(false);
                controlInfo->setIX(false);
                controlInfo->setMXaDst(false);
                controlInfo->setDCNC(false);
                controlInfo->setUC(true);
                controlInfo->setSplit(true);
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
                controlInfo->setHopCount(-1);
                flit->setPacket(packet);

                m_queue.insert(flit);
            }

            m_nextPacketId++;

        }

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
