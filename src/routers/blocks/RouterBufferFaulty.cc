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

        m_temp_bitflip_bits.resize(32, 0);

    }


    RouterBuffer::initialize(stage);

    double d = 0.6768;
    m_P3.push_back(std::make_pair(d, std::make_pair(1,1)));
    d += 0.1521;
    m_P3.push_back(std::make_pair(d, std::make_pair(2,1)));
    d += 0.0353;
    m_P3.push_back(std::make_pair(d, std::make_pair(3,1)));
    d += 0.0090;
    m_P3.push_back(std::make_pair(d, std::make_pair(4,1)));
    d += 0.0019;
    m_P3.push_back(std::make_pair(d, std::make_pair(5,1)));
    d += 0.0003;
    m_P3.push_back(std::make_pair(d, std::make_pair(6,1)));
    d += 0.0001;
    m_P3.push_back(std::make_pair(d, std::make_pair(7,1)));
    d += 0.0836;
    m_P3.push_back(std::make_pair(d, std::make_pair(1,2)));
    d += 0.0195;
    m_P3.push_back(std::make_pair(d, std::make_pair(2,2)));
    d += 0.0047;
    m_P3.push_back(std::make_pair(d, std::make_pair(3,2)));
    d += 0.0009;
    m_P3.push_back(std::make_pair(d, std::make_pair(4,2)));
    d += 0.0002;
    m_P3.push_back(std::make_pair(d, std::make_pair(5,2)));
    d += 0.0001;
    m_P3.push_back(std::make_pair(d, std::make_pair(6,2)));
    d += 0.0102;
    m_P3.push_back(std::make_pair(d, std::make_pair(1,3)));
    d += 0.0028;
    m_P3.push_back(std::make_pair(d, std::make_pair(2,3)));
    d += 0.0007;
    m_P3.push_back(std::make_pair(d, std::make_pair(3,3)));
    d += 0.0002;
    m_P3.push_back(std::make_pair(d, std::make_pair(4,3)));
    d += 0.0013;
    m_P3.push_back(std::make_pair(d, std::make_pair(1,4)));
    d += 0.0003;
    m_P3.push_back(std::make_pair(d, std::make_pair(2,4)));

}

bool RouterBufferFaulty::sampleFault() {
    double sample = double(rand())/RAND_MAX;
    return (sample <= 32.0*double(par("p_bitflip_link")));
}

// bits, time
RouterBufferFaulty::Pentry_t RouterBufferFaulty::sampleFaultCharacteristic() {
    double sample = double(rand())/RAND_MAX;

    for (P_t::iterator it = m_P3.begin(); it != m_P3.end(); ++it) {
        std::pair<double, Pentry_t> t = *it;
        if (sample <= t.first) {
            return t.second;
        }
    }

    ASSERT(false);
    return Pentry_t(-1,-1);
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
        int flipped = 0;
        //for(int i=0; i<32; i++) {
            bool fault = sampleFault();
            if (fault) {
                int bit = rand() % 32;
                std::cout << "[" << simTime() << "," << getFullPath() << "] FAULT!!!" << std::endl;
                RouterBufferFaulty::Pentry_t e = sampleFaultCharacteristic();
                Pw_t w = e.first;
                Pt_t t = e.second;

                std::pair<int, int> range = getNeighborRange(bit, w);
                std::cout << "[" << simTime() << "," << getFullPath() << "] -> flip bits " << range.first << " to " << (range.second-1) << std::endl;
                for (int b = range.first; b < range.second; b++) {
                    errorVector ^= (1 << b);
                    flipped |= (1 << b);
                    ASSERT(t>0);
                    m_temp_bitflip_bits[b] = t-1;
                }
                //break; // We don't need another fault..
            }
        //}

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
