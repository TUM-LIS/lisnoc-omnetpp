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

#ifndef __LISNOC_ROUTERSWITCH_H_
#define __LISNOC_ROUTERSWITCH_H_

#include <omnetpp.h>
#include <LISNoCMessages.h>

namespace lisnoc {

/**
 * TODO - Generated class
 */
class RouterSwitch : public cSimpleModule
{
public:
    virtual ~RouterSwitch();
private:
    int m_nVCs;
    int m_nPorts;
    cMessage *m_selfSignal;

    class Arbiter {
        int m_port;
        int m_vc;
        int m_nPorts;
        std::vector<bool> m_requests;
        std::vector<bool> m_requestsIsHead;
        std::vector<bool> m_requestsIsTail;
        bool m_pending;
        bool m_outputReady;
        bool m_pendingOutputReply;

        bool m_transmittingWorm;
        int m_arbitratedPort;

    public:
        Arbiter(int numports, int port, int vc);
        void request(int port, int vc, bool head, bool tail);
        bool pendingArbitration();
        int arbitrate();
        void setOutputReady(bool ready);
        bool getOutputReady();
    };

    std::vector<std::vector<Arbiter*> > m_outputArbiters;
    std::vector<std::vector<LISNoCFlowControlRequest*> > m_outputRequests;

    std::vector<std::vector<LISNoCFlitControlInfo*> > m_inputRequests;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void handleMessageRequest(LISNoCFlowControlRequest *msg);
    virtual void handleMessageGrant(LISNoCFlowControlGrant *msg);
    virtual void handleMessageFlit(LISNoCFlit *msg);
    virtual void arbitrate();
};

} //namespace

#endif
