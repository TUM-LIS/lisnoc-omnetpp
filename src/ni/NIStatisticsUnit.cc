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

#include "NIStatisticsUnit.h"
#include <GlobalStatisticsUnit.h>

namespace lisnoc {

Define_Module(NIStatisticsUnit);

#define CONV_INTERVAL simtime_t(500, SIMTIME_US)

void NIStatisticsUnit::initialize()
{
    GlobalStatisticsUnit::s_getGlobalStatisticsUnit()->registerNIStatisticsUnit(par("id"), this);

    m_packetCount = 0;
    m_faultyPacketCount = 0;

    m_seperateStream = par("seperateStream");

    if ((m_seperateStream == -1) || (m_seperateStream == int(par("id")))) {
        scheduleAt(CONV_INTERVAL, new cMessage);
    }
}

void NIStatisticsUnit::handleMessage(cMessage *msg)
{
    ASSERT(msg->isSelfMessage());
    delete msg;

    double conv = double(m_faultyPacketCount) / double(m_packetCount);

    m_faultProbability.collect(conv);

    std::cout << simTime() << " " << getFullPath() << " CONVERGENCE: " << conv << " STDDEV: " << m_faultProbability.getStddev() << std::endl;

    scheduleAt(simTime() + CONV_INTERVAL, new cMessage);
}

void NIStatisticsUnit::collectFlitLatency(int networkAccessLatency, int networkLatency) {
    m_flitNetworkAccessLatency.collect(networkAccessLatency);
    m_flitNetworkLatency.collect(networkLatency);
    m_flitTotalLatency.collect(networkAccessLatency+networkLatency);
}

void NIStatisticsUnit::reportFlitArrivedFaulty(bool faulty) {
    m_flitArrivedFaulty.collect(faulty);
}

void NIStatisticsUnit::reportPacketArrivedFaulty(bool faulty) {
    m_packetCount++;
    if (faulty) m_faultyPacketCount++;
    m_packetArrivedFaulty.collect(faulty);
}

void NIStatisticsUnit::finish() {
    m_flitNetworkLatency.recordAs("flit_network_latency");
    m_flitNetworkAccessLatency.recordAs("flit_network_access_latency");
    m_flitTotalLatency.recordAs("flit_total_latency");
    m_flitArrivedFaulty.recordAs("flit_arrived_faulty");
    m_packetArrivedFaulty.recordAs("packet_arrived_faulty");
}

} //namespace
