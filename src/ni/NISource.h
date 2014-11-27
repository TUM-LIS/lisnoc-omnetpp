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

#ifndef __LISNOC_NISOURCE_H
#define __LISNOC_NISOURCE_H

#include <omnetpp.h>

#include <LISNoC_m.h>
#include <LISNoCBaseModule.h>

namespace lisnoc {

/**
 * Generates messages; see NED file for more info.
 */
class NISource : public LISNoCBaseModule
{
protected:
    virtual ~NISource();
  private:
    int m_id;
    cMessage *m_timerMessage;
    cQueue m_queue;
    int m_abortAccessLatency;
    int m_nextPacketId;

  protected:
    virtual void initialize();
    virtual void genPacket();
    virtual void handleSelfMessage(cMessage *msg);
    virtual void doTransfer();
    virtual bool isRequestGranted(LISNoCFlowControlRequest *msg) { return false; }
    virtual void handleIncomingFlit(LISNoCFlit *msg) { }

};

}; // namespace

#endif
