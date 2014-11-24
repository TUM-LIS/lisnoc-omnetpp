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

// Created: 2014-10-24 for project lisnoc

package lisnoc.networks;

import lisnoc.routers.buffered.RouterBuffered;
import lisnoc.ni.NetworkInterface;

network MeshBuffered {
	parameters:
	    int columns = default(4);
	    int rows = default(4);
	    int nPorts = 5;
	    int nVCs = default(1);
	    
	submodules:
	    router[columns*rows]: RouterBuffered {
	    	parameters:
	    	    nPorts = nPorts;
	    	    nVCs = nVCs;   
	    }
	    
	    ni[columns*rows]: NetworkInterface {
	     	parameters:
	     	    nVCs = nVCs;   
	    }
	    
	connections allowunconnected:
	    
	    // LOCAL = 0, NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4
	    for r=0..(rows-1), for c=0..(columns-1), for vc=0..(nVCs-1) {
	    	router[r*columns+c].out[1*nVCs+vc] --> router[(r-1)*columns+c].in[3*nVCs+vc] if r!=0;
	    	router[r*columns+c].out[2*nVCs+vc] --> router[r*columns+c-1].in[4*nVCs+vc] if c!=0;
	    	router[r*columns+c].out[3*nVCs+vc] --> router[(r+1)*columns+c].in[1*nVCs+vc] if r!=rows-1;
	    	router[r*columns+c].out[4*nVCs+vc] --> router[r*columns+c+1].in[2*nVCs+vc] if c!=columns-1;
	    	router[r*columns+c].fc_out[1*nVCs+vc] --> router[(r-1)*columns+c].fc_in[3*nVCs+vc] if r!=0;
	    	router[r*columns+c].fc_out[2*nVCs+vc] --> router[r*columns+c-1].fc_in[4*nVCs+vc] if c!=0;
	    	router[r*columns+c].fc_out[3*nVCs+vc] --> router[(r+1)*columns+c].fc_in[1*nVCs+vc] if r!=rows-1;
	    	router[r*columns+c].fc_out[4*nVCs+vc] --> router[r*columns+c+1].fc_in[2*nVCs+vc] if c!=columns-1;
	    	
	    	ni[r*columns+c].out[vc] --> router[r*columns+c].in[0*nVCs+vc];
	    	router[r*columns+c].out[0*nVCs+vc] --> ni[r*columns+c].in[vc];
	    	ni[r*columns+c].fc_out[vc] --> router[r*columns+c].fc_in[0*nVCs+vc];
	    	router[r*columns+c].fc_out[0*nVCs+vc] --> ni[r*columns+c].fc_in[vc];
	    }         
}
