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

#include "RouterSwitch.h"

namespace lisnoc {

Define_Module(RouterSwitch);

void RouterSwitch::initialize()
{
    m_nVCs = par("nVCs");
    m_nPorts = par("nPorts");

    m_requests.resize(m_nPorts);
    for (int p = 0; p < m_nPorts; p++) {
        m_requests[p].resize(m_nVCs);
    }
}

void RouterSwitch::handleMessageRequest(LISNoCFlowControlMsg *msg)
{
    LISNoCFlowControlMsg *req = &m_requests[msg->getOutputPort()][msg->getVC()];
    if (!reg->isScheduled()) {
        // This port has not been requested before in this cycle
        send(req, "fc_req_out", msg->getVC());
    }
}

void RouterSwitch::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {

    } else if (msg->getKind() == LISNOC_REQUEST) {

    }
}

} //namespace
