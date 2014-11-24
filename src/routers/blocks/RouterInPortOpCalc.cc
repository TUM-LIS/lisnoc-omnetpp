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
#define NEXT_FALLING (simTime() + simtime_t(1,SIMTIME_NS))
#define NEXT_RISING (simTime() + simtime_t(1,SIMTIME_NS))

namespace lisnoc {

Define_Module(RouterInPortOpCalc);

void RouterInPortOpCalc::initialize()
{
    m_storedFlit = NULL;

    // Todo: static XY selection for now
    m_routingFunction = new RoutingFunctionMeshXY(4, getIndex());
}

void RouterInPortOpCalc::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        trySend();
    } else if (msg->getKind() == LISNOC_FLIT) {
        handleIncomingFlit((LISNoCFlit*) msg);
    } else if (msg->getKind() == LISNOC_RESPONSE) {
        handleIncomingResponse((LISNoCResponse*) msg);
    }
}

void RouterInPortOpCalc::handleIncomingFlit(LISNoCFlit *msg) {

    LISNoCResponse *resp = new LISNoCResponse();

    if(m_storedFlit == NULL) {
        m_storedFlit = msg;

        // do routing
        m_routingFunction->doRouting(msg);

        resp->setAck(true);
        scheduleAt(NEXT_CYCLE, &m_timerMsg);
    } else {
        resp->setAck(false);
    }

    sendDelayed(resp,NEXT_FALLING,"out_fc");
}

void RouterInPortOpCalc::handleIncomingResponse(LISNoCResponse *msg) {

    assert(m_storedFlit != NULL);

    if (msg->getAck()) {
        m_storedFlit = NULL;
    } else {
        scheduleAt(NEXT_RISING, &m_timerMsg);
    }

}

void RouterInPortOpCalc::trySend() {
    assert(m_storedFlit != NULL);
    send(m_storedFlit, "out");
}

} //namespace
