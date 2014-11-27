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

namespace lisnoc;

enum LISNoC_Message {
	LISNOC_FLIT = 0;
	LISNOC_REQUEST = 1;
	LISNOC_GRANT = 2;
}

class LISNoCFlitControlInfo {
	int VC;
	int outputPort;
    int packetId;
  	int flitId;
	bool isHead;
	bool isTail;    
	int srcId;
  	int dstId;
};

packet LISNoCFlit {
    @customize(true);
  	uint8_t data[];
  	simtime_t generationTime;
  	simtime_t sendTime;
  	int errorVector;
}

packet LISNoCFlowControlRequest {
	@customize(true);
    bool allowLateAck = false;
}

packet LISNoCFlowControlGrant {
    @customize(true);
    bool ack = false;
	bool isLateAck = false;
}	
