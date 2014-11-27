/*
 * LISNoCPacket.h
 *
 *  Created on: Nov 27, 2014
 *      Author: andreas
 */

#ifndef LISNOCPACKET_H_
#define LISNOCPACKET_H_

class LISNoCPacket {
  public:
    LISNoCPacket(int id) {
        m_id = id;
        m_isFaulty = false;
    }

    int getId() {
        return m_id;
    }

    void setFaulty() {
        m_isFaulty = true;
    }

    bool isFaulty() {
        return m_isFaulty;
    }

  private:
    int m_id;
    bool m_isFaulty;
};



#endif /* LISNOCPACKET_H_ */
