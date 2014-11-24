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

#include "RouterInPortOpCalc.h"
#include "RoutingFunctionMeshXY.h"

#include <cassert>

#define NEXT_CYCLE (simTime() + simtime_t(2,SIMTIME_NS))

namespace lisnoc {

Define_Module(RouterInPortOpCalc);

void RouterInPortOpCalc::initialize()
{
    m_storedFlit = NULL;

    // Todo: static XY selection for now
    m_routingFunction = new RoutingFunctionMeshXY(2, getIndex());
}

void RouterInPortOpCalc::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        trySend();
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleIncomingFlit((LISNoCFlit*) msg);
    } else if (msg->getKind() == LISNOC_REQUEST) {
        handleIncomingRequest((LISNoCFlowControlMsg*) msg);
    } else if (msg->getKind() == LISNOC_GRANT) {
        handleIncomingGrant((LISNoCFlowControlMsg*) msg);
    }
}

void RouterInPortOpCalc::handleIncomingFlit(LISNoCFlit *msg)
{
    ASSERT(m_storedFlit == NULL);

    m_storedFlit = msg;

    // do routing
    m_routingFunction->doRouting(msg);

}

void RouterInPortOpCalc::handleIncomingGrant(LISNoCFlowControlMsg *msg)
{
    ASSERT(m_storedFlit != NULL);

    send(m_storedFlit, "out");
    m_storedFlit = NULL;
}

void RouterInPortOpCalc::handleIncomingRequest(LISNoCFlowControlMsg *msg)
{
    if(m_storedFlit == NULL) {
        msg->setKind(LISNOC_GRANT);
        sendDelayed(msg, SIMTIME_ZERO, "fc_grant_out");
    }
}

void RouterInPortOpCalc::trySend() {
    ASSERT(m_storedFlit != NULL);

    m_flowControlMessage.setKind(LISNOC_REQUEST);
    sendDelayed(&m_flowControlMessage, SIMTIME_ZERO, "fc_req_out");
}

} //namespace
