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

#ifndef __LISNOC_NISTATISTICSUNIT_H_
#define __LISNOC_NISTATISTICSUNIT_H_

#include <omnetpp.h>

namespace lisnoc {

class NIStatisticsUnit : public cSimpleModule
{
  public:

    void collectFlitLatency(int networkAccessLatency, int networkLatency);
    void reportFlitArrivedFaulty(bool faulty);
    void reportPacketArrivedFaulty(bool faulty);


    cLongHistogram* getHistFlitNetworkAccessLatency() {
        return &m_flitNetworkAccessLatency;
    }

    cLongHistogram* getHistFlitNetworkLatency() {
        return &m_flitNetworkLatency;
    }

    cLongHistogram* getHistFlitTotalLatency() {
        return &m_flitTotalLatency;
    }

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

  private:
    cLongHistogram m_flitNetworkAccessLatency;
    cLongHistogram m_flitNetworkLatency;
    cLongHistogram m_flitTotalLatency;
    cHistogram m_flitArrivedFaulty;
    cHistogram m_packetArrivedFaulty;

    unsigned int m_packetCount;
    unsigned int m_faultyPacketCount;
    cStdDev m_faultProbability;

    int m_seperateStream;
};

} //namespace

#endif
