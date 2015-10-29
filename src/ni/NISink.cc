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

#include "NISink.h"
#include "NIStatisticsUnit.h"
#include <GlobalStatisticsUnit.h>

namespace lisnoc {

Define_Module(NISink);

int NISink::numInitStages() const {
    return 2;
}

void NISink::initialize(int stage)
{
    if(stage == 0) {
        LISNoCBaseModule::initialize();
    } else if(stage == 1) {
        GlobalStatisticsUnit* globalSU =
                GlobalStatisticsUnit::s_getGlobalStatisticsUnit();

        m_niSU =
                globalSU->getNIStatisticsUnit(par("id"));
    }

    do_xor_idx = -1;
}

void NISink::handleIncomingFlit(LISNoCFlit *msg)
{
   // std::cout << "[" << simTime() << "," << getFullPath() << "] Received flit" << std::endl;

    LISNoCFlitControlInfo *ctrlInfo = (LISNoCFlitControlInfo*) msg->getControlInfo();

    int src1 = ctrlInfo->getSrcId();
    int src2 = ctrlInfo->getSrc2Id();
    int dst1 = ctrlInfo->getDstId();
    int dst2 = ctrlInfo->getDst2Id();
    int flit_id = ctrlInfo->getFlitId();

    //std::cout<<"SINK= "<<unsigned(par("id"))<<", Source = "<<src1<<", Flit ID: "<<flit_id<<", Generation Time: "<<msg->getGenerationTime()<<endl;

    if (ctrlInfo->getMXaDst()) {

        if (ctrlInfo->getIsHead()) {

            for (int i=0; i<(int)to_XOR_list.size(); i++) {
                if ((to_XOR_list[i].src1 == src2) && (to_XOR_list[i].src2 == src1) && (to_XOR_list[i].dst1 == dst1) && (to_XOR_list[i].dst2 == dst2)) {
                    for(int k=0; k<4 ; k++){
                        msg->setData(k, unsigned(msg->getData(k)^to_XOR_list[i].data_bytes[k]));
                        //std::cout<<"MY_ID: "<<unsigned(par("id"))<<"Source1: "<<src1<<", Source2: "<<src2<<"XORED Data: "<<   unsigned(msg->getData(k))<<endl;
                    }

                    do_xor_idx = i;

                    break;
                }
            }
            if (do_xor_idx == -1) {
                packet_info_t temp;
                temp.src1 = src1;
                temp.src2 = src2;
                temp.dst1 = dst1;
                temp.dst2 = dst2;
                temp.data_bytes.push_back(msg->getData(0));
                temp.data_bytes.push_back(msg->getData(1));
                temp.data_bytes.push_back(msg->getData(2));
                temp.data_bytes.push_back(msg->getData(3));

                to_XOR_list.push_back(temp);
            }
        } else {
            // either add to xor_list or directly xor with xor_list data
            if (do_xor_idx != -1) {
                for(int k=0; k<4 ; k++){
                    msg->setData(k, unsigned(msg->getData(k)^to_XOR_list[do_xor_idx].data_bytes[flit_id*4 + k]));
                    //std::cout<<"MY_ID: "<<unsigned (par("id"))<<"Source1: "<<src1<<", Source2: "<<src2<<", XORED Data: "<< unsigned(msg->getData(k))<<endl;
                }
            } else {
                for (int k=0; k<4; k++) {
                    to_XOR_list[to_XOR_list.size()-1].data_bytes.push_back(msg->getData(k));
                }
            }
        }
    }

    m_niSU->collectFlitLatency(
            (msg->getSendTime()-msg->getGenerationTime()).inUnit(SIMTIME_NS),
            (simTime()-msg->getSendTime()).inUnit(SIMTIME_NS)
            );

    m_niSU->collectFlitHopCount(ctrlInfo->getHopCount());

    if(ctrlInfo->getIsTail()) {
       if (ctrlInfo->getMXaDst()) {
           if (do_xor_idx != -1) {
               to_XOR_list.erase(to_XOR_list.begin() + do_xor_idx);
               do_xor_idx = -1;
           }
       }

       delete msg->getPacket();
    }

    delete(msg);
}

bool NISink::isRequestGranted(LISNoCFlowControlRequest *msg)
{
    return true;
}

}; // namespace
