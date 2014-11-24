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

simple RouterSwitch
{
    parameters:
        int nPorts;
        int nVCs;
    
    @display("i=block/switch");
        
    gates:
        input in[nPorts*nVCs];
        output out[nPorts*nVCs];
        
        input fc_req_in[nPorts*nVCs];
        input fc_grant_in[nPorts*nVCs];
        
        output fc_req_out[nPorts*nVCs];
        output fc_grant_out[nPorts*nVCs];
        
}
