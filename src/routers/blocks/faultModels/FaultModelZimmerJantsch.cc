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

            m_p_faultLink = par("p_faultLink");
            m_p_faultBuffer = par("p_faultLink"); // TODO: Quick and Dirty

            std::vector<double> weights;
            weights.push_back(0.6768); m_P3.push_back(std::make_pair(1,1));
            weights.push_back(0.1521); m_P3.push_back(std::make_pair(2,1));
            weights.push_back(0.0353); m_P3.push_back(std::make_pair(3,1));
            weights.push_back(0.0090); m_P3.push_back(std::make_pair(4,1));
            weights.push_back(0.0019); m_P3.push_back(std::make_pair(5,1));
            weights.push_back(0.0003); m_P3.push_back(std::make_pair(6,1));
            weights.push_back(0.0001); m_P3.push_back(std::make_pair(7,1));
            weights.push_back(0.0836); m_P3.push_back(std::make_pair(1,2));
            weights.push_back(0.0195); m_P3.push_back(std::make_pair(2,2));
            weights.push_back(0.0047); m_P3.push_back(std::make_pair(3,2));
            weights.push_back(0.0009); m_P3.push_back(std::make_pair(4,2));
            weights.push_back(0.0002); m_P3.push_back(std::make_pair(5,2));
            weights.push_back(0.0001); m_P3.push_back(std::make_pair(6,2));
            weights.push_back(0.0102); m_P3.push_back(std::make_pair(1,3));
            weights.push_back(0.0028); m_P3.push_back(std::make_pair(2,3));
            weights.push_back(0.0007); m_P3.push_back(std::make_pair(3,3));
            weights.push_back(0.0002); m_P3.push_back(std::make_pair(4,3));
            weights.push_back(0.0013); m_P3.push_back(std::make_pair(1,4));
            weights.push_back(0.0003); m_P3.push_back(std::make_pair(2,4));

            m_sampleFaultDistributionLink = new std::discrete_distribution<int64_t>({m_p_faultLink, 1.0 - m_p_faultLink});
            m_sampleFaultDistributionBuffer = new std::discrete_distribution<int64_t>({m_p_faultBuffer, 1.0 - m_p_faultBuffer});
            m_sampleCharacteristicDistribution = new std::discrete_distribution<int64_t>(weights.begin(), weights.end());
        }


        FaultModelBase::initialize(stage);
    }

    bool FaultModelZimmerJantsch::sampleFaultLink() {
        static std::mt19937_64  generator;

        for (int i = 0; i < 32; i++) {
            if ((*m_sampleFaultDistributionLink)(generator) == 0) {
                return true;
            }
        }
        return false;
    }

    bool FaultModelZimmerJantsch::sampleFaultBuffer() {
        static std::mt19937_64 generator;

        for (int i = 0; i < 32; i++) {
            if ((*m_sampleFaultDistributionBuffer)(generator) == 0) {
                return true;
            }
        }
        return false;
    }

    void FaultModelZimmerJantsch::sampleFaultCharacteristics(std::vector<struct FaultCharacteristics> &faultVector) {
        static std::mt19937_64 generator;
        faultVector.clear();

        unsigned int sampleIndex = (*m_sampleCharacteristicDistribution)(generator);
        ASSERT(sampleIndex < m_P3.size());
        Pentry_t sample = m_P3[sampleIndex];

        struct FaultCharacteristics fault;
        fault.wire = rand() % 32;
        fault.numWires = sample.first;
        fault.duration = sample.second;
        fault.effect = INVERSION;
        faultVector.push_back(fault);

    }

} /* namespace lisnoc */
