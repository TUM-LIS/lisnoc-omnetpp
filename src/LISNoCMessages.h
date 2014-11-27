/*
 * LISNoCMessages.h
 *
 *  Created on: Nov 26, 2014
 *      Author: gu45zin
 */

#ifndef LISNOCMESSAGES_H_
#define LISNOCMESSAGES_H_

#include <LISNoC_m.h>
#include <LISNoCPacket.h>

namespace lisnoc {

    class LISNoCFlit : public LISNoCFlit_Base
     {
       private:
        void copy(const LISNoCFlit& other) { }
        LISNoCPacket *m_packet;
       public:
         LISNoCFlit(const char *name=NULL, int kind=0) : LISNoCFlit_Base(name,LISNOC_FLIT), m_packet(NULL) {}
         LISNoCFlit(const LISNoCFlit& other) : LISNoCFlit_Base(other) {copy(other);}
         LISNoCFlit& operator=(const LISNoCFlit& other) {if (this==&other) return *this; LISNoCFlit_Base::operator=(other); copy(other); return *this;}
         virtual LISNoCFlit *dup() const {return new LISNoCFlit(*this);}
         void setPacket(LISNoCPacket *packet) { m_packet = packet; }
         LISNoCPacket* getPacket() { return m_packet; }
     };

    class LISNoCFlowControlRequest : public LISNoCFlowControlRequest_Base
    {
    private:
        void copy(const LISNoCFlowControlRequest& other) { }

    public:
        LISNoCFlowControlRequest(const char *name=NULL, int kind=0) : LISNoCFlowControlRequest_Base(name,LISNOC_REQUEST) {}
        LISNoCFlowControlRequest(const LISNoCFlowControlRequest& other) : LISNoCFlowControlRequest_Base(other) {copy(other);}
        ~LISNoCFlowControlRequest() {  }
        LISNoCFlowControlRequest& operator=(const LISNoCFlowControlRequest& other) {if (this==&other) return *this; LISNoCFlowControlRequest_Base::operator=(other); copy(other); return *this;}
        virtual LISNoCFlowControlRequest *dup() const {return new LISNoCFlowControlRequest(*this);}
    };

    class LISNoCFlowControlGrant : public LISNoCFlowControlGrant_Base
    {
    private:
        void copy(const LISNoCFlowControlGrant& other) { }
    public:
        LISNoCFlowControlGrant(const char *name=NULL, int kind=0) : LISNoCFlowControlGrant_Base(name,LISNOC_GRANT) {}
        LISNoCFlowControlGrant(const LISNoCFlowControlGrant& other) : LISNoCFlowControlGrant_Base(other) {copy(other);}
        ~LISNoCFlowControlGrant() {  }
        LISNoCFlowControlGrant& operator=(const LISNoCFlowControlGrant& other) {if (this==&other) return *this; LISNoCFlowControlGrant_Base::operator=(other); copy(other); return *this;}
        virtual LISNoCFlowControlGrant *dup() const {return new LISNoCFlowControlGrant(*this);}
    };


} // namespace: lisnoc

#endif /* LISNOCMESSAGES_H_ */
