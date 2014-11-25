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

#include "RouterStatisticsUnit.h"
#include <GlobalStatisticsUnit.h>

namespace lisnoc {

Define_Module(RouterStatisticsUnit);

void RouterStatisticsUnit::initialize(int stage)
{
    if(stage == 0) {
        GlobalStatisticsUnit::s_getGlobalStatisticsUnit()->registerRouterStatisticsUnit(par("routerId"), this);

        m_nPorts = par("nPorts");
        m_nVCs = par("nVCs");

        m_inBufferLat.resize(m_nPorts);
        m_outBufferLat.resize(m_nPorts);

        for(int p=0; p<m_nPorts; p++) {
            m_inBufferLat[p].resize(m_nVCs);
            m_outBufferLat[p].resize(m_nVCs);
        }
    }
}

void RouterStatisticsUnit::handleMessage(cMessage *msg)
{
    // should never be called
    ASSERT(false);
}

void RouterStatisticsUnit::collectInBufferLatency(int port, int vc, simtime_t latency) {
    m_inBufferLat[port][vc].collect(latency);
}

void RouterStatisticsUnit::collectOutBufferLatency(int port, int vc, simtime_t latency) {
    m_outBufferLat[port][vc].collect(latency);
}

void RouterStatisticsUnit::finish() {
    char recordName[64];
    for(int p=0; p<m_nPorts; p++) {
        for(int vc=0; vc<m_nVCs; vc++) {
            sprintf(recordName, "inBuffer_%i_vc_%i_latency", p, vc);
            m_inBufferLat[p][vc].recordAs(recordName);

            sprintf(recordName, "outBuffer_%i_vc_%i_latency", p, vc);
            m_outBufferLat[p][vc].recordAs(recordName);
        }
    }
}

} //namespace
