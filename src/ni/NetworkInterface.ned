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

package lisnoc.ni;

//
// TODO auto-generated type
//
module NetworkInterface
{
    parameters:
        int nVCs;
    
    gates:
        input in[nVCs];
        output out[nVCs];
        output fc_out[nVCs];
        input fc_in[nVCs];
        
    submodules:
        sink: Sink {
            @display("p=43,48");
        }
        source: Source {
            @display("p=109,48");
        }
        
    connections:
	    for vc=0..(nVCs-1) {
			source.out[vc] --> out[vc];
			fc_in[vc] --> source.fc_in[vc];
			sink.fc_out[vc] --> fc_out[vc];
			in[vc] --> sink.in[vc];
   	    }    
        
}
