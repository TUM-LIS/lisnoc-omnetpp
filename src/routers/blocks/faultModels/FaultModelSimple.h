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

#ifndef FAULTMODELSIMPLE_H_
#define FAULTMODELSIMPLE_H_

#include <FaultModel.h>

namespace lisnoc {

class FaultModelSimple: public lisnoc::FaultModelBase {
private:
    double m_p_bitFlip;
    unsigned int m_currentFlipVector;
protected:
    virtual void initialize(int stage);
public:
    virtual bool sampleFault();
    virtual void sampleFaultCharacteristics(std::vector<struct FaultCharacteristics> &faultVector);
};

} /* namespace lisnoc */

#endif /* FAULTMODELSIMPLE_H_ */
