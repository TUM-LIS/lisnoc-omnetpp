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

#include "RouterInPortVcCalc.h"
#include <LISNoC_m.h>

namespace lisnoc {

Define_Module(RouterInPortVcCalc);

void RouterInPortVcCalc::initialize()
{

}

void RouterInPortVcCalc::handleMessage(cMessage *msg)
{
    if (msg->getKind() == LISNOC_FLIT) {
        send(msg, "out");
    } else if (msg->getKind() == LISNOC_REQUEST) {
        send(msg, "fc_req_out");
    } else if (msg->getKind() == LISNOC_GRANT) {
        send(msg, "fc_grant_out");
    }
}

} //namespace
