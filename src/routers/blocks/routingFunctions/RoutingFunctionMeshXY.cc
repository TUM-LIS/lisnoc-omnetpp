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
    //int dst2Id = controlInfo->getDst2Id();
    int dst3Id = controlInfo->getDst3Id();
    int IDId = controlInfo->getDst4Id_ID();
    bool DCNC = controlInfo->getDCNC();
    bool MXaIS = controlInfo->getMXaIS();
    bool iX = controlInfo->getIX();
    int dstPosX = dstId%m_columns;
    int dstPosY = dstId/m_columns;
    bool UC = controlInfo->getUC();

    int outputPort;

    if (UC==true){
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
        return;
    }

    if (DCNC==false){
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
    return;
    }

    if(DCNC==true){
        if (MXaIS==false){
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
        }
        else{
            if(iX==false){
            if(dstPosX > m_routerPosX) {
                outputPort = 2; // EAST
            } else if(dstPosX < m_routerPosX) {
                outputPort = 4; // WEST
            } else if(dstPosY > m_routerPosY) {
                outputPort = 3; // SOUTH
            } else if(dstPosY < m_routerPosY) {
                outputPort = 1; // NORTH
            } else {
                if ((dst3Id==-1) && (IDId==-1)){
                    outputPort = 0;
                }
                else{
                    int IDPosX = IDId%m_columns;
                    int IDPosY = IDId/m_columns;

                    if(IDPosX > m_routerPosX) {
                        outputPort = 2; // EAST
                    } else if(IDPosX < m_routerPosX) {
                        outputPort = 4; // WEST
                    } else if(IDPosY > m_routerPosY) {
                        outputPort = 3; // SOUTH
                    } else if(IDPosY < m_routerPosY) {
                        outputPort = 1; // NORTH
                    } else {
                        outputPort = 0; // LOCAL
                    }

                    //controlInfo->setDstId(IDId);

                }
            }
        }
            else{
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
            }
        }


    controlInfo->setOutputPort(outputPort);
    }
}
}
