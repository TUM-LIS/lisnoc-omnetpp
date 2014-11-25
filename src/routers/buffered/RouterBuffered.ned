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

package lisnoc.routers.buffered;

import lisnoc.routers.blocks.RouterInPort;
import lisnoc.routers.blocks.RouterOutPort;
import lisnoc.routers.blocks.RouterSwitch;
import lisnoc.routers.blocks.RouterStatisticsUnit;

module RouterBuffered
{
    parameters:
        int nPorts;
        int nVCs;
        int id;
        
    @display("i=abstract/router");
    gates:
        input in[nPorts*nVCs];
        output out[nPorts*nVCs];
        
        input fc_req_in[nPorts*nVCs];
        input fc_grant_in[nPorts*nVCs];
        
        output fc_req_out[nPorts*nVCs];
        output fc_grant_out[nPorts*nVCs];
        
   	submodules:
   		inPort[nPorts]: RouterInPort {
   			parameters:
   			    nVCs = nVCs;
   			    routerId = id;
   			    portId = index;  
   		}
   		outPort[nPorts]: RouterOutPort {
   		 	parameters:
   		 	    nVCs = nVCs;
   		 	    routerId = id;
   		 	    portId = index;
   		}
   		switch: RouterSwitch {
   		 	parameters:
   		 	    nPorts = nPorts;
   		 	    nVCs = nVCs;   
   		}
   		statisticsUnit: RouterStatisticsUnit {
   		 	parameters:
   		 	    routerId = id;
   		 	    nPorts = nPorts;
   		 	    nVCs = nVCs;   
   		}
   		
   	connections:
	    for p=0..(nPorts-1), for vc=0..(nVCs-1) {
			in[p*nVCs+vc] --> inPort[p].in[vc];
			inPort[p].out[vc] --> switch.in[p*nVCs+vc];
			switch.out[p*nVCs+vc] --> outPort[p].in[vc];
			outPort[p].out[vc] --> out[p*nVCs+vc];
			
			fc_req_in[p*nVCs+vc] --> inPort[p].fc_req_in[vc];
			inPort[p].fc_req_out[vc] --> switch.fc_req_in[p*nVCs+vc];
			switch.fc_req_out[p*nVCs+vc] --> outPort[p].fc_req_in[vc];
			outPort[p].fc_req_out[vc] --> fc_req_out[p*nVCs+vc];
			
			fc_grant_in[p*nVCs+vc] --> outPort[p].fc_grant_in[vc];
			outPort[p].fc_grant_out[vc] --> switch.fc_grant_in[p*nVCs+vc];
			switch.fc_grant_out[p*nVCs+vc] --> inPort[p].fc_grant_in[vc];
			inPort[p].fc_grant_out[vc] --> fc_grant_out[p*nVCs+vc];			
   	    }    
}
