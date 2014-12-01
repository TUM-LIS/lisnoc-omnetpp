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

void RouterStatisticsUnit::initialize()
{
    GlobalStatisticsUnit::s_getGlobalStatisticsUnit()->registerRouterStatisticsUnit(par("routerId"), this);

    m_nPorts = par("nPorts");
    m_nVCs = par("nVCs");

    m_inBufferLat.resize(m_nPorts);
    m_outBufferLat.resize(m_nPorts);

    m_inBufferFault.resize(m_nPorts);
    m_outBufferFault.resize(m_nPorts);
    m_linkFault.resize(m_nPorts);

    m_inBufferBitflip.resize(m_nPorts);
    m_outBufferBitflip.resize(m_nPorts);
    m_linkBitflip.resize(m_nPorts);


    for(int p=0; p<m_nPorts; p++) {
        m_inBufferLat[p].resize(m_nVCs);
        m_outBufferLat[p].resize(m_nVCs);

        m_inBufferFault[p].resize(m_nVCs);
        m_outBufferFault[p].resize(m_nVCs);
        m_linkFault[p].resize(m_nVCs);

        m_inBufferBitflip[p].resize(m_nVCs);
        m_outBufferBitflip[p].resize(m_nVCs);
        m_linkBitflip[p].resize(m_nVCs);
    }
}

void RouterStatisticsUnit::handleMessage(cMessage *msg)
{
    // should never be called
    ASSERT(false);
}

void RouterStatisticsUnit::collectBufferLatency(const char *type, int port, int vc, int latency) {
    ASSERT(initialized());
    if (strcmp("in",type) == 0) {
        m_inBufferLat[port][vc].collect(latency);
    } else {
        m_outBufferLat[port][vc].collect(latency);
    }
}

void RouterStatisticsUnit::collectFault(const char* type, int port, int vc) {
    if (strcmp("in", type) == 0) {
        m_inBufferFault[port][vc].collect(1);
    } else if (strcmp("out", type) == 0) {
        m_outBufferFault[port][vc].collect(1);
    } else if (strcmp("link", type) == 0){
        m_linkFault[port][vc].collect(1);
    } else {
        error("Unknown type");
    }
}

void RouterStatisticsUnit::collectBitflip(const char* type, int port, int vc) {
    if (strcmp("in", type) == 0) {
        m_inBufferBitflip[port][vc].collect(1);
    } else if (strcmp("out", type) == 0) {
        m_outBufferBitflip[port][vc].collect(1);
    } else if (strcmp("link", type) == 0){
        m_linkBitflip[port][vc].collect(1);
    } else {
        error("Unknown type");
    }
}


void RouterStatisticsUnit::finish() {
    char recordName[64];
    for(int p=0; p<m_nPorts; p++) {
        for(int vc=0; vc<m_nVCs; vc++) {
            sprintf(recordName, "inBuffer_%i_vc_%i_latency", p, vc);
            m_inBufferLat[p][vc].recordAs(recordName);

            sprintf(recordName, "outBuffer_%i_vc_%i_latency", p, vc);
            m_outBufferLat[p][vc].recordAs(recordName);

            sprintf(recordName, "inBuffer_%i_vc_%i_faults", p, vc);
            m_inBufferFault[p][vc].recordAs(recordName);

            sprintf(recordName, "outBuffer_%i_vc_%i_faults", p, vc);
            m_outBufferFault[p][vc].recordAs(recordName);

            sprintf(recordName, "link_%i_vc_%i_faults", p, vc);
            m_linkFault[p][vc].recordAs(recordName);

            sprintf(recordName, "inBuffer_%i_vc_%i_bitflips", p, vc);
            m_inBufferBitflip[p][vc].recordAs(recordName);

            sprintf(recordName, "outBuffer_%i_vc_%i_bitflips", p, vc);
            m_outBufferBitflip[p][vc].recordAs(recordName);

            sprintf(recordName, "link_%i_vc_%i_bitflips", p, vc);
            m_linkBitflip[p][vc].recordAs(recordName);
        }
    }
}

} //namespace
