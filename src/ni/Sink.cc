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

#include "Sink.h"

namespace lisnoc {

Define_Module(Sink);

void Sink::initialize()
{

}

void Sink::handleMessageFlit(LISNoCFlit *msg)
{
    std::cout << "[" << simTime() << "," << getFullPath() << "] Received flit" << std::endl;
}

void Sink::handleMessageRequest(LISNoCFlowControlMsg *msg)
{
   msg->setKind(LISNOC_GRANT);
   sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out");
}

void Sink::handleMessage(cMessage *msg)
{
    if (msg->getKind() == LISNOC_REQUEST) {
        handleMessageRequest((LISNoCFlowControlMsg*) msg);
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleMessageFlit((LISNoCFlit*) msg);
    }
}

void Sink::finish()
{

}

}; // namespace
