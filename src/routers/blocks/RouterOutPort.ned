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

package lisnoc.routers.blocks;

module RouterOutPort
{
    parameters:
        int nVCs;
        int routerId;
        int portId;

    @display("i=block/join");
        
    gates:
        input in[nVCs];
        output out[nVCs];
        
        input fc_req_in[nVCs];
        input fc_grant_in[nVCs];
        
        output fc_req_out[nVCs];
        output fc_grant_out[nVCs];
        
    submodules:
        buffer[nVCs]: RouterBuffer {
   	        parameters:
   	            type = "out";
   	            routerId = routerId;
   	            portId = portId;
   	            vcId = index;
   	    }
        
    connections:
        for i=0..(nVCs-1) {
         	in[i] --> buffer[i].in;
         	buffer[i].out --> out[i];
         	
         	fc_req_in[i] --> buffer[i].fc_req_in;
         	buffer[i].fc_req_out --> fc_req_out[i];
         	
         	fc_grant_in[i] --> buffer[i].fc_grant_in;
         	buffer[i].fc_grant_out --> fc_grant_out[i];
        }            
}
