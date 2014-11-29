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

#include "NISinkFaulty.h"
#include "NIStatisticsUnit.h"

namespace lisnoc {

Define_Module(NISinkFaulty);

void NISinkFaulty::handleIncomingFlit(LISNoCFlit *msg)
{
    LISNoCFlitControlInfo* ctrlInfo = (LISNoCFlitControlInfo*) msg->getControlInfo();
    if (((ctrlInfo->getSrcId() == 0) && (ctrlInfo->getDstId() == 60)) || (int(par("seperateStream")) == 0)) {
        int errorVector = msg->getErrorVector();
        m_niSU->reportFlitArrivedFaulty(errorVector != 0);

        if(errorVector != 0) {
            msg->getPacket()->setFaulty();
        }

        if(ctrlInfo->getIsTail()) {
            m_niSU->reportPacketArrivedFaulty(msg->getPacket()->isFaulty());
        }
    }
    NISink::handleIncomingFlit(msg);
}

} //namespace
