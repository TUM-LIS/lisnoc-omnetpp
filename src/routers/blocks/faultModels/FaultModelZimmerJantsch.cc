/*
 * FaultModelZimmerJantsch.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: gu45zin
 */

#include <FaultModelZimmerJantsch.h>

namespace lisnoc {

    Define_Module(FaultModelZimmerJantsch);

    void FaultModelZimmerJantsch::initialize(int stage) {
        if (stage == 0) {
            ASSERT(int(par("example"))==3);

            m_p_fault = par("p_fault");

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

        FaultModelBase::initialize(stage);
    }

    bool FaultModelZimmerJantsch::sampleFault() {
        for (int i = 0; i < 32; i++) {
            double sample = double(rand())/RAND_MAX;
            if (sample <= m_p_fault) {
                return true;
            }
        }
        return false;
    }

    void FaultModelZimmerJantsch::sampleFaultCharacteristics(std::vector<struct FaultCharacteristics> &faultVector) {
        faultVector.clear();
        double sample = double(rand())/RAND_MAX;

        for (P_t::iterator it = m_P3.begin(); it != m_P3.end(); ++it) {
            std::pair<double, Pentry_t> t = *it;
            if (sample <= t.first) {
                struct FaultCharacteristics fault;
                fault.wire = rand() % 32;
                fault.numWires = t.second.first;
                fault.duration = t.second.second;
                fault.effect = INVERSION;
                faultVector.push_back(fault);
                break;
            }
        }

        ASSERT(faultVector.size() > 0);
    }

} /* namespace lisnoc */
