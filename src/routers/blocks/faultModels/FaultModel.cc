
#include "FaultModel.h"

namespace lisnoc {

FaultModelRegistry* FaultModelRegistry::s_registry = NULL;

FaultModelRegistry* FaultModelRegistry::s_getFaultModelRegistry() {
    if (s_registry == NULL) {
        s_registry = new FaultModelRegistry;
    }
    return s_registry;
}

void FaultModelRegistry::registerRouterFaultModel(int routerId, FaultModelBase* faultModel) {
    ASSERT(m_routerFaultModels.find(routerId) == m_routerFaultModels.end());
    m_routerFaultModels[routerId] = faultModel;
}

FaultModelBase *FaultModelRegistry::getRouterFaultModel(int routerId) {
    ASSERT(m_routerFaultModels.find(routerId) != m_routerFaultModels.end());
    ASSERT(m_routerFaultModels[routerId]);
    return m_routerFaultModels[routerId];
}

void FaultModelBase::initialize(int stage)
{
    ASSERT(stage == 0);
    FaultModelRegistry::s_getFaultModelRegistry()->registerRouterFaultModel(par("routerId"), this);
}

void FaultModelBase::handleMessage(cMessage *msg)
{
    ASSERT(false);
}

void FaultModelBase::finish()
{

}

}; // namespace
