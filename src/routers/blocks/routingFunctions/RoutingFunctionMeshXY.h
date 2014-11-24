/*
 * RoutingFunctionMeshXY.h
 *
 *  Created on: Nov 24, 2014
 *      Author: gu32xih
 */

#ifndef ROUTINGFUNCTIONMESHXY_H_
#define ROUTINGFUNCTIONMESHXY_H_

#include "RoutingFunctionBase.h"

namespace lisnoc {

class RoutingFunctionMeshXY : public RoutingFunctionBase {
public:
    RoutingFunctionMeshXY(int columns, int routerId);
    virtual void doRouting(LISNoCFlit *flit);

private:
    int m_columns;

    int m_routerPosX;
    int m_routerPosY;
};

}



#endif /* ROUTINGFUNCTIONMESHXY_H_ */
