/*
 * faultModel.h
 *
 *  Created on: Nov 28, 2014
 *      Author: gu45zin
 */

#ifndef FAULTMODEL_H_
#define FAULTMODEL_H_

#include <omnetpp.h>

namespace lisnoc {

    typedef enum FaultEffect {
        INVERSION,
        STUCKATZERO,
        STUCKATONE,
        RANDOM,
        BRIDGE,
        DELAY
    } FaultEffect;

    struct FaultCharacteristics {
        int wire;
        int numWires;
        int duration;
        FaultEffect effect;
    };

    class FaultModelBase : public cSimpleModule
    {
    protected:
        virtual void initialize(int stage);
        virtual int getNumStages() const { return 1; }
        virtual void handleMessage(cMessage *msg);
        virtual void finish();
    public:
        virtual bool sampleFaultLink() = 0;
        virtual bool sampleFaultBuffer() = 0;
        virtual void sampleFaultCharacteristics(std::vector<struct FaultCharacteristics> &faultVector) = 0;
    };

    class FaultModelRegistry
    {
    public:
        static FaultModelRegistry* s_getFaultModelRegistry();

        void registerRouterFaultModel(int routerId, FaultModelBase* faultModel);
        FaultModelBase *getRouterFaultModel(int routerId);
    private:
        std::map<int, FaultModelBase*> m_routerFaultModels;
        FaultModelRegistry() {}
        static FaultModelRegistry *s_registry;

    };

}; // namespace

#endif /* FAULTMODEL_H_ */
