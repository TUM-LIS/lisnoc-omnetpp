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

#ifndef __LISNOC_SOURCE_H
#define __LISNOC_SOURCE_H

#include <omnetpp.h>

#include <LISNoC_m.h>

namespace lisnoc {

/**
 * Generates messages; see NED file for more info.
 */
class Source : public cSimpleModule
{
  private:
    int m_id;
    cMessage m_timerMessage;
    cMessage m_trySendMessage;
    LISNoCFlowControlMsg m_flowControlMessage;
    cQueue m_queue;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void handleMessageGrant(LISNoCFlowControlMsg *msg);
    virtual void genPacket();
    virtual void trySend();
};

}; // namespace

#endif
