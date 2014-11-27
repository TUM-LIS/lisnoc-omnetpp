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

#include "RouterBufferFaulty.h"
#include <cmath>

namespace lisnoc {

Define_Module(RouterBufferFaulty);

void RouterBufferFaulty::initialize(int stage) {
    if(stage == 0) {
        m_p_bitflip_buffer = par("p_bitflip_buffer");
        m_p_bitflip_link = par("p_bitflip_link");


        // TODO: add parameter
        m_p_bitflip_link_next_cycle = 0.5;
        m_temp_bitflip_window = 5;

        for(int i=0; i<32; i++) {
            m_nContBitFlips[i] = 0;
        }

    }

    RouterBuffer::initialize(stage);


}

void RouterBufferFaulty::doTransfer() {

    LISNoCFlit *flit = (LISNoCFlit*) m_buffer.front();
    int bufferTime = (simTime().inUnit(SIMTIME_NS) - flit->getArrivalTime().inUnit(SIMTIME_NS));

    double pBitFlip;
    double randVal;

    int errorVector = flit->getErrorVector();

    // for the link
    for(int i=0; i<32; i++) {
        if(m_nContBitFlips[i] == 0) {
            pBitFlip = m_p_bitflip_link;
        } else {
            pBitFlip = std::pow(m_p_bitflip_link_next_cycle, m_nContBitFlips[i]);
        }

        randVal = (double)rand() / RAND_MAX;

        if(randVal < pBitFlip) {
            errorVector ^= (1 << i);
            m_nContBitFlips[i]++;
            m_nContBitFlips[i] %= m_temp_bitflip_window;
        } else {
            m_nContBitFlips[i] = 0;
        }
    }


//    // first for the buffer
//    for(int i=0; i<32; i++) {
//        for(int j=0; j<bufferTime; j++) {
//
//            randVal = (double)rand() / RAND_MAX;
//
//            if(randVal < m_p_bitflip_buffer) {
//                errorVector ^= (1 << i);
//            }
//        }
//    }
//
//    // same for the link behind the output port
//    if(strcmp(m_type, "out") == 0) {
//        for(int i=0; i<32; i++) {
//            randVal = (double)rand() / RAND_MAX;
//
//            if(randVal < m_p_bitflip_link) {
//                errorVector ^= (1 << i);
//            }
//        }
//    }

    flit->setErrorVector(errorVector);

    RouterBuffer::doTransfer();

}

} //namespace
