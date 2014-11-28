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
#include "FaultModel.h"

#include <cmath>
#include <random>

namespace lisnoc {

Define_Module(RouterBufferFaulty);

void RouterBufferFaulty::initialize(int stage) {
    if(stage == 0) {
        m_p_bitflip_buffer = par("p_bitflip_buffer");
        m_p_bitflip_link = par("p_bitflip_link");

        m_temp_bitflip_bits.resize(32, 0);

    } else if (stage == 1) {
        m_faultmodel = FaultModelRegistry::s_getFaultModelRegistry()->getRouterFaultModel(par("routerId"));
    }


    RouterBuffer::initialize(stage);


}

std::pair<int, int> RouterBufferFaulty::getNeighborRange(int bit, int count) {
    int begin = std::max(0, bit - int(double(count)/2));
    int end = std::min(32, begin + count);
    if ((end - begin) != count) {
        begin = end - count;
    }
    ASSERT((begin+count)==end);
    ASSERT(begin <= bit);
    ASSERT(end >= bit);
    ASSERT(begin >= 0);
    ASSERT(end <= 32);
    return std::make_pair(begin, end);
}


void RouterBufferFaulty::doTransfer() {

    LISNoCFlit *flit = (LISNoCFlit*) m_buffer.front();
    //int bufferTime = (simTime().inUnit(SIMTIME_NS) - flit->getArrivalTime().inUnit(SIMTIME_NS));

    int errorVector = flit->getErrorVector();

    // for the link
    if(strcmp(m_type, "out") == 0) {
        std::vector<struct FaultCharacteristics> faults;
        int flipped = 0;
        bool fault = m_faultmodel->sampleFault();
        if (fault) {
            std::cout << "[" << simTime() << "," << getFullPath() << "] FAULT!!!" << std::endl;

            m_faultmodel->sampleFaultCharacteristics(faults);

            for (std::vector<struct FaultCharacteristics>::iterator it = faults.begin(); it != faults.end(); ++it) {
                std::pair<int, int> range = getNeighborRange(it->wire, it->numWires);
                std::cout << "[" << simTime() << "," << getFullPath() << "] -> flip bits " << range.first << " to " << (range.second-1) << std::endl;
                for (int b = range.first; b < range.second; b++) {
                    errorVector ^= (1 << b);
                    flipped |= (1 << b);
                    ASSERT(it->duration > 0);
                    m_temp_bitflip_bits[b] = it->duration - 1;
                }
            }
        }

        for (int i = 0; i < 32; i++) {
            if (flipped & (1 << i)) {
                continue;
            }

            if (m_temp_bitflip_bits[i] > 0) {
                std::cout << "[" << simTime() << "," << getFullPath() << "] Temporal flip of bit " << i << std::endl;
                errorVector ^= (1 << i);
                m_temp_bitflip_bits[i]--;
            }
        }
    }

    flit->setErrorVector(errorVector);

    RouterBuffer::doTransfer();

}

} //namespace
