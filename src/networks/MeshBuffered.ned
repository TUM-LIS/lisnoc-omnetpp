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

import ned.DelayChannel;

module Node
{
    parameters:
        @display("i=misc/node_vs");
    gates:
        inout g[];
    connections allowunconnected:
}

//
// A generated network with grid topology.
//
network MeshBuffered
{
    parameters:
        int columns = default(4);
        int rows = default(4);
    submodules:
        node[columns*rows]: Node {
            @display("p=,,matrix,$columns");
            gates:
                g[4];
        }
    connections allowunconnected:
        for i=0..rows-1, for j=0..columns-1 {
            node[i*columns+j].g[1] <--> DelayChannel <--> node[(i+1)*columns+j].g[0] if i!=rows-1;
            node[i*columns+j].g[3] <--> DelayChannel <--> node[i*columns+j+1].g[2] if j!=columns-1;
        }
}
