/*
 * LISNoCMessages.h
 *
 *  Created on: Nov 26, 2014
 *      Author: gu45zin
 */

#ifndef LISNOCMESSAGES_H_
#define LISNOCMESSAGES_H_

#include <LISNoC_m.h>

namespace lisnoc {

    class LISNoCFlit : public LISNoCFlit_Base
     {
       private:
        void copy(const LISNoCFlit& other) { ASSERT(false); }
       public:
         LISNoCFlit(const char *name=NULL, int kind=0) : LISNoCFlit_Base(name,LISNOC_FLIT) {}
         LISNoCFlit(const LISNoCFlit& other) : LISNoCFlit_Base(other) {copy(other);}
         LISNoCFlit& operator=(const LISNoCFlit& other) {if (this==&other) return *this; LISNoCFlit_Base::operator=(other); copy(other); return *this;}
         virtual LISNoCFlit *dup() const {return new LISNoCFlit(*this);}
     };

    class LISNoCFlowControlRequest : public LISNoCFlowControlRequest_Base
    {
    private:
        void copy(const LISNoCFlowControlRequest& other) { ASSERT(false); }

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
        void copy(const LISNoCFlowControlGrant& other) { ASSERT(false); }
    public:
        LISNoCFlowControlGrant(const char *name=NULL, int kind=0) : LISNoCFlowControlGrant_Base(name,LISNOC_GRANT) {}
        LISNoCFlowControlGrant(const LISNoCFlowControlGrant& other) : LISNoCFlowControlGrant_Base(other) {copy(other);}
        ~LISNoCFlowControlGrant() {  }
        LISNoCFlowControlGrant& operator=(const LISNoCFlowControlGrant& other) {if (this==&other) return *this; LISNoCFlowControlGrant_Base::operator=(other); copy(other); return *this;}
        virtual LISNoCFlowControlGrant *dup() const {return new LISNoCFlowControlGrant(*this);}
    };


} // namespace: lisnoc

#endif /* LISNOCMESSAGES_H_ */
