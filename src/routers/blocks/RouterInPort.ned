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

module RouterInPort
{
    parameters:
        int nVCs;
        
    gates:
        input in[nVCs];
        output out[nVCs];
        
   	submodules:
   	    buffer[nVCs]: RouterBuffer;
   	    opCalc[nVCs]: RouterInPortOpCalc;
   	    vcCalc[nVCs]: RouterInPortVcCalc;
   	    
   	 connections:
   	     for i=0..(nVCs-1) {
   	     	in[i] --> buffer[i].in;
   	     	buffer[i].out --> opCalc[i].in;
   	     	opCalc[i].out --> vcCalc[i].in;
   	     	vcCalc[i].out --> out[i];
   	     }   	     
}
