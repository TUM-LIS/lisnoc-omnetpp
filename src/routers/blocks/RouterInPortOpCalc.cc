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

namespace lisnoc {

Define_Module(RouterInPortOpCalc);

void RouterInPortOpCalc::initialize()
{
    LISNoCBaseModule::initialize();

    m_storedFlit = NULL;

    // Todo: static XY selection for now
    m_routingFunction = new RoutingFunctionMeshXY(int(par("columns")), par("routerId"));
}

bool RouterInPortOpCalc::isRequestGranted(LISNoCFlowControlRequest *msg)
{
    return (m_storedFlit == NULL);
}

void RouterInPortOpCalc::handleIncomingFlit(LISNoCFlit *msg)
{
    ASSERT(m_storedFlit == NULL);

    m_storedFlit = msg;

    // do routing
    m_routingFunction->doRouting(msg);

    triggerSelf(1);
}

void RouterInPortOpCalc::handleSelfMessage(cMessage *msg)
{
    trySend();
}

void RouterInPortOpCalc::trySend() {
    ASSERT(m_storedFlit != NULL);

    requestTransfer(m_storedFlit);
}

void RouterInPortOpCalc::doTransfer()
{
    ASSERT(m_storedFlit != NULL);

    send(m_storedFlit, "out");
    m_storedFlit = NULL;

    tryLateGrant();
}

RouterInPortOpCalc::~RouterInPortOpCalc()
{
    cancelEvent(&m_timerMsg);
    delete m_storedFlit;
}

} //namespace
