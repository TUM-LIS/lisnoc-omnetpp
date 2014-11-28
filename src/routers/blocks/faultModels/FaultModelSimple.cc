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

#include <FaultModelSimple.h>

namespace lisnoc {

    Define_Module(FaultModelSimple);

    void FaultModelSimple::initialize(int stage) {
        ASSERT(stage == 0);
        m_p_bitFlip = par("p_bitFlip");
        FaultModelBase::initialize(stage);

        m_sampleDistribution = new std::discrete_distribution<>({m_p_bitFlip,1.0 - m_p_bitFlip});
    }

    bool FaultModelSimple::sampleFault() {
        static std::default_random_engine generator;
        m_currentFlipVector = 0;

        // TODO: bitwidth
        for (int i = 0; i < 32; i++) {

            if ((*m_sampleDistribution)(generator) == 0) {
                m_currentFlipVector |= (1 << i);
            }
        }
        return (m_currentFlipVector != 0);
    }

    void FaultModelSimple::sampleFaultCharacteristics(std::vector<struct FaultCharacteristics> &faultVector) {
        faultVector.clear();

        for (int i = 0; i < 32; i++) {
            if (m_currentFlipVector & (1 << i)) {
                struct FaultCharacteristics fault;
                fault.wire = i;
                fault.numWires = 1;
                fault.duration = 1;
                fault.effect = INVERSION;

                faultVector.push_back(fault);
            }
        }
    }

} /* namespace lisnoc */
