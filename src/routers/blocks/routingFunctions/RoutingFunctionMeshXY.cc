/*
 * RoutingFunctionMeshXY.cc
 *
 *  Created on: Nov 24, 2014
 *      Author: gu32xih
 */

#include "RoutingFunctionMeshXY.h"
#include <LISNoCMessages.h>

namespace lisnoc {

RoutingFunctionMeshXY::RoutingFunctionMeshXY(int columns, int routerId)
    : m_columns(columns)
{
    m_routerPosX = routerId%columns;
    m_routerPosY = routerId/columns;
}

void RoutingFunctionMeshXY::doRouting(LISNoCFlit *flit) {
    LISNoCFlitControlInfo *controlInfo = (LISNoCFlitControlInfo *) flit->getControlInfo();
    int dstId = controlInfo->getDstId();
    int dstPosX = dstId%m_columns;
    int dstPosY = dstId/m_columns;

    int outputPort;

    if(dstPosX > m_routerPosX) {
        outputPort = 2; // EAST
    } else if(dstPosX < m_routerPosX) {
        outputPort = 4; // WEST
    } else if(dstPosY > m_routerPosY) {
        outputPort = 3; // SOUTH
    } else if(dstPosY < m_routerPosY) {
        outputPort = 1; // NORTH
    } else {
        outputPort = 0; // LOCAL
    }

    controlInfo->setOutputPort(outputPort);
}

}
