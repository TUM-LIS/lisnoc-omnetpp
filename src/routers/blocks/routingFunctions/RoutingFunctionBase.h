/*
 * RoutingFunctionBase.h
 *
 *  Created on: Nov 24, 2014
 *      Author: gu32xih
 */

#ifndef ROUTINGFUNCTIONBASE_H_
#define ROUTINGFUNCTIONBASE_H_

namespace lisnoc {

class LISNoCFlit;

class RoutingFunctionBase {
public:
    virtual void doRouting(LISNoCFlit *flit)=0;
};

}



#endif /* ROUTINGFUNCTIONBASE_H_ */
