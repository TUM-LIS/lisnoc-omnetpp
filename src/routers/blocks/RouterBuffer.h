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

#ifndef __LISNOC_ROUTERBUFFER_H_
#define __LISNOC_ROUTERBUFFER_H_

#include <omnetpp.h>

#include <LISNoC_m.h>

namespace lisnoc {

/**
 * TODO - Generated class
 */
class RouterBuffer : public cSimpleModule
{
private:
    cQueue m_buffer;
    int m_maxfill;
    cMessage m_timerMsg;
    LISNoCFlowControlMsg m_flowControlMsg;
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
