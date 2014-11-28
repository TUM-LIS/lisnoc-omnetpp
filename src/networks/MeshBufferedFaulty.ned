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

import lisnoc.routers.buffered.RouterBufferedFaulty;
import lisnoc.ni.NIFaulty;
import lisnoc.GlobalStatisticsUnit;

network MeshBufferedFaulty {
	parameters:
	    int columns = default(4);
	    int rows = default(4);
	    int nPorts = 5;
	    int nVCs = default(1);
	    
	submodules:
	    router[columns*rows]: RouterBufferedFaulty {
	    	parameters:
	    	    nPorts = nPorts;
	    	    nVCs = nVCs;
	    	    id = index;   
	        @display("p=100,100,m,$columns,100,100");
	    }
	    
	    ni[columns*rows]: NIFaulty {
	     	parameters:
	     	    nVCs = nVCs;
	     	    numNIs = columns*rows; 
	     	    id = index;
	        @display("p=50,50,m,$columns,100,100");
	    }
	    
	    globalStatitics: GlobalStatisticsUnit;
	    
	connections allowunconnected:
	    
	    // LOCAL = 0, NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4
	    for r=0..(rows-1), for c=0..(columns-1), for vc=0..(nVCs-1) {
	    	router[r*columns+c].out[1*nVCs+vc] --> router[(r-1)*columns+c].in[3*nVCs+vc] if r!=0;
	    	router[r*columns+c].out[2*nVCs+vc] --> router[r*columns+c+1].in[4*nVCs+vc] if c!=columns-1;
	    	router[r*columns+c].out[3*nVCs+vc] --> router[(r+1)*columns+c].in[1*nVCs+vc] if r!=rows-1;
	    	router[r*columns+c].out[4*nVCs+vc] --> router[r*columns+c-1].in[2*nVCs+vc] if c!=0;
	    	
	    	router[r*columns+c].fc_req_out[1*nVCs+vc] --> router[(r-1)*columns+c].fc_req_in[3*nVCs+vc] if r!=0;
	    	router[r*columns+c].fc_req_out[2*nVCs+vc] --> router[r*columns+c+1].fc_req_in[4*nVCs+vc] if c!=columns-1;
	    	router[r*columns+c].fc_req_out[3*nVCs+vc] --> router[(r+1)*columns+c].fc_req_in[1*nVCs+vc] if r!=rows-1;
	    	router[r*columns+c].fc_req_out[4*nVCs+vc] --> router[r*columns+c-1].fc_req_in[2*nVCs+vc] if c!=0;
	    	
	    	router[r*columns+c].fc_grant_out[1*nVCs+vc] --> router[(r-1)*columns+c].fc_grant_in[3*nVCs+vc] if r!=0;
	    	router[r*columns+c].fc_grant_out[2*nVCs+vc] --> router[r*columns+c+1].fc_grant_in[4*nVCs+vc] if c!=columns-1;
	    	router[r*columns+c].fc_grant_out[3*nVCs+vc] --> router[(r+1)*columns+c].fc_grant_in[1*nVCs+vc] if r!=rows-1;
	    	router[r*columns+c].fc_grant_out[4*nVCs+vc] --> router[r*columns+c-1].fc_grant_in[2*nVCs+vc] if c!=0;
	    	
	        router[r*columns+c].out[0*nVCs+vc] --> ni[r*columns+c].in[vc];
	    	router[r*columns+c].fc_req_out[0*nVCs+vc] --> ni[r*columns+c].fc_req_in[vc];
	    	ni[r*columns+c].fc_grant_out[vc] --> router[r*columns+c].fc_grant_in[0*nVCs+vc];
	
	    	ni[r*columns+c].out[vc] --> router[r*columns+c].in[0*nVCs+vc];
	    	ni[r*columns+c].fc_req_out[vc] --> router[r*columns+c].fc_req_in[0*nVCs+vc];
	    	router[r*columns+c].fc_grant_out[0*nVCs+vc] --> ni[r*columns+c].fc_grant_in[vc];	    	
	    }         
}
