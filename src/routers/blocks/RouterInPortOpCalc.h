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

#ifndef __LISNOC_ROUTERINPORTOPCALC_H_
#define __LISNOC_ROUTERINPORTOPCALC_H_

#include <omnetpp.h>

#include <LISNoC_m.h>

namespace lisnoc {

class RoutingFunctionBase;

class RouterInPortOpCalc : public cSimpleModule
{
private:
    LISNoCFlit *m_storedFlit;
    cMessage m_timerMsg;

    RoutingFunctionBase *m_routingFunction;

    LISNoCFlowControlMsg m_flowControlMessage;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void handleIncomingFlit(LISNoCFlit *msg);
    virtual void handleIncomingRequest(LISNoCFlowControlMsg *msg);
    virtual void handleIncomingGrant(LISNoCFlowControlMsg *msg);
    virtual void trySend();
};

} //namespace

#endif
