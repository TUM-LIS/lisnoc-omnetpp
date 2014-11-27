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

#ifndef __LISNOC_ROUTERBUFFERFAULTY_H_
#define __LISNOC_ROUTERBUFFERFAULTY_H_

#include "RouterBuffer.h"

namespace lisnoc {

/**
 * TODO - Generated class
 */
class RouterBufferFaulty : public RouterBuffer
{
  protected:
    virtual void initialize(int stage);
    virtual void doTransfer();

  private:
    double m_p_bitflip_buffer;
    double m_p_bitflip_link;
    double m_p_bitflip_link_next_cycle;
    int m_temp_bitflip_window;

    int m_nContBitFlips[32]; // one value for each wire of the link

};

} //namespace

#endif
