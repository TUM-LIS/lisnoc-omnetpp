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

//#define FLITS_PER_PACKET 4          // TODO: later link with par("flitsPerPacket")

namespace lisnoc {

typedef struct {
    unsigned int data_bytes[4];
    simtime_t send_time;
    simtime_t gen_time;
} flit_info_t;

typedef struct {
    std::vector<flit_info_t> flit_info;
} m_data_to_XOR_t;



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
    int m_flitsperpacket;

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
    //std::vector<std::vector<LISNoCFlit*> > m_data_to_XOR;
    std::vector<m_data_to_XOR_t> m_data_to_XOR;
    int SrcDest [5][4];                  //for each port save 2 sources and destinations for XOR

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
