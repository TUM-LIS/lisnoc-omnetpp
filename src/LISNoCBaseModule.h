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

#ifndef LISNOCBASEMODULE_H_
#define LISNOCBASEMODULE_H_

#include <csimplemodule.h>

#include <LISNoC_m.h>

namespace lisnoc {

class LISNoCBaseModule: public cSimpleModule {
public:
    virtual ~LISNoCBaseModule();
    virtual void finish();
private:
    bool m_allowLateAck;
    LISNoCFlowControlMsg *m_flowControlMsg;
    LISNoCFlowControlMsg *m_incomingFlowControlMsg;
    cMessage *m_selfTrigger;
    simtime_t m_clock;

    std::pair<bool, simtime_t> m_pendingRequestWithLateAck;

    bool m_isInitialized;
protected:
    virtual void initialize();
    virtual void allowLateAck();

    virtual void handleMessage(cMessage *msg);
    virtual void handleIncomingRequest(LISNoCFlowControlMsg *msg);
    virtual void handleIncomingGrant(LISNoCFlowControlMsg *msg);
    virtual void handleIncomingFlit(LISNoCFlit *msg) = 0;

    virtual void triggerSelf(unsigned int numcycles = 1, cMessage *msg = NULL);
    virtual void handleSelfMessage(cMessage *msg) = 0;

    virtual bool canRequestTransfer(LISNoCFlit *msg);
    virtual void requestTransfer(LISNoCFlit *msg);
    virtual void requestTransferAfter(LISNoCFlit *msg, unsigned int numcycles);
    virtual void doTransfer() = 0;

    virtual bool isRequestGranted(LISNoCFlowControlMsg *msg) = 0;
    virtual void tryLateGrant();
};

} /* namespace lisnoc */

#endif /* LISNOCBASEMODULE_H_ */
