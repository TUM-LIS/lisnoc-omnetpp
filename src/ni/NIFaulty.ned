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
module NIFaulty
{
    parameters:
        int id;
        int nVCs;
        int numNIs;

        @display("i=device/cpu");

    gates:
        // NI output
        output out[nVCs];
        output fc_req_out[nVCs];
        input fc_grant_in[nVCs];

		// NI input
        input in[nVCs];
        input fc_req_in[nVCs];
        output fc_grant_out[nVCs];

    submodules:
        sink: NISinkFaulty {
            parameters:
                id = id;
                @display("p=43,48");
        }
        source: NISource {
            parameters:
                id = id;
                @display("p=109,48");
        }
        statisticsUnit: NIStatisticsUnit {
            parameters:
                id = id;
                @display("p=232,70");
        }

    connections:
        for vc=0..(nVCs-1) {
            source.out[vc] --> out[vc];
            source.fc_req_out[vc] --> fc_req_out[vc];
            fc_grant_in[vc] --> source.fc_grant_in[vc];
            in[vc] --> sink.in[vc];
            fc_req_in[vc] --> sink.fc_req_in[vc];
            sink.fc_grant_out[vc] --> fc_grant_out[vc];
        }

}
