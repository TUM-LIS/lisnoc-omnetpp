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

void NIStatisticsUnit::initialize()
{
    GlobalStatisticsUnit::s_getGlobalStatisticsUnit()->registerNIStatisticsUnit(par("id"), this);
}

void NIStatisticsUnit::handleMessage(cMessage *msg)
{
    // should never happen
    ASSERT(false);
}

void NIStatisticsUnit::collectFlitLatency(int networkAccessLatency, int networkLatency) {
    m_flitNetworkAccessLatency.collect(networkAccessLatency);
    m_flitNetworkLatency.collect(networkLatency);
    m_flitTotalLatency.collect(networkAccessLatency+networkLatency);
}


void NIStatisticsUnit::finish() {
    m_flitNetworkLatency.recordAs("flit_network_latency");
    m_flitNetworkAccessLatency.recordAs("flit_network_access_latency");
    m_flitTotalLatency.recordAs("flit_total_latency");
}

} //namespace