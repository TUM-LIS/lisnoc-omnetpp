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

#ifndef __LISNOC_ROUTERSTATISTICSUNIT_H_
#define __LISNOC_ROUTERSTATISTICSUNIT_H_

#include <omnetpp.h>

namespace lisnoc {

class RouterStatisticsUnit : public cSimpleModule
{
  public:
    void collectInBufferLatency(simtime_t latency);
    void collectOutBufferLatency(simtime_t latency);

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

  private:

    cLongHistogram m_inBufferLat;
    cLongHistogram m_outBufferLat;
};

} //namespace

#endif
