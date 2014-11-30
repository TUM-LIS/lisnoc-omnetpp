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
// Generates messages with a configurable interarrival time.
//
simple NISource
{
    parameters:
        volatile double sendInterval @unit(s) = default(exponential(1s));
        int nVCs = default(1);
        int id;
        volatile int dstId = default(-1);
        volatile int initialdelay = default(0);
        volatile int genDelay = default(100);
        int abortAccessLatency = default(1000);
        volatile int flitsPerPacket = default(8);
    @display("i=block/source");
    gates:
        output out[nVCs];
        output fc_req_out[nVCs];
        input fc_grant_in[nVCs];
}
