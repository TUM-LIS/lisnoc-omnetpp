/*
 * Copyright (c) 2014 by the authors
 *
 * Authors:
 *   Stefan Wallentowitz <stefan.wallentowitz@tum.de>
 *   Andreas Oeldemann <andreas.oeldemann@tum.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LISNOCBASEMODULE_H_
#define LISNOCBASEMODULE_H_

#include <csimplemodule.h>

#include <LISNoCMessages.h>

namespace lisnoc {

  /**
   * Basic flow control handling
   *
   * This class provides the designer the basic flow control protocol
   * handling as employed by all modules. The class itself is
   * abstract, i.e., it cannot be instantiated. You need to derive
   * your module from this class and implement the following function:
   *
   * - `void handleIncomingFlit(LISNoCFlit *msg)`: Handle an incoming
   *     message after successfull flow control handshake
   *
   *
   */
  class LISNoCBaseModule: public cSimpleModule {
  public:
    virtual ~LISNoCBaseModule();
    virtual void finish();
  private:
    bool m_allowLateAck;
    cMessage *m_selfTrigger;
    LISNoCFlowControlRequest *m_delayedTransfer;
    simtime_t m_clock;
    
    bool m_activeRequest;
    std::pair<LISNoCFlowControlRequest*, simtime_t> m_pendingRequestWithLateAck;
    
    bool m_isInitialized;

    virtual LISNoCFlowControlRequest *createFlowControlRequest(LISNoCFlit *flit);
    virtual LISNoCFlowControlRequest *createFlowControlRequest(LISNoCFlowControlGrant *grant);
    virtual LISNoCFlowControlGrant *createFlowControlGrant(LISNoCFlowControlRequest *request);

  protected:
    virtual void initialize();
    virtual void allowLateAck();

    virtual void handleMessage(cMessage *msg);
    virtual void handleIncomingRequest(LISNoCFlowControlRequest *msg);
    virtual void handleIncomingGrant(LISNoCFlowControlGrant *msg);
    virtual void handleIncomingFlit(LISNoCFlit *msg) = 0;
    
    virtual void triggerSelf(unsigned int numcycles = 1, cMessage *msg = NULL);
    virtual void handleSelfMessage(cMessage *msg) = 0;
    
    virtual bool canRequestTransfer(LISNoCFlit *msg);
    virtual void requestTransfer(LISNoCFlit *msg);
    virtual void requestTransferAfter(LISNoCFlit *msg, unsigned int numcycles);
    virtual void delayedTransfer();
    virtual void doTransfer() = 0;
    
    virtual bool isRequestGranted(LISNoCFlowControlRequest *msg) = 0;
    virtual void tryLateGrant();
  };
  
} /* namespace lisnoc */

#endif /* LISNOCBASEMODULE_H_ */
