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

#include "GlobalStatisticsUnit.h"
#include <NIStatisticsUnit.h>

namespace lisnoc {

GlobalStatisticsUnit* GlobalStatisticsUnit::s_globalStatisticsUnit = NULL;

Define_Module(GlobalStatisticsUnit);

GlobalStatisticsUnit::GlobalStatisticsUnit() {
    ASSERT(s_globalStatisticsUnit == NULL);
    s_globalStatisticsUnit = this;
}

void GlobalStatisticsUnit::registerRouterStatisticsUnit(int routerId, RouterStatisticsUnit *routerSU) {
    std::map<int,RouterStatisticsUnit*>::const_iterator iter =
            m_routerStatisticsUnits.find(routerId);

    ASSERT(iter == m_routerStatisticsUnits.end());

    std::pair<int,RouterStatisticsUnit*> p =
            std::make_pair(routerId, routerSU);

    m_routerStatisticsUnits.insert(p);
}

void GlobalStatisticsUnit::registerNIStatisticsUnit(int niId, NIStatisticsUnit *niSU) {
    std::map<int,NIStatisticsUnit*>::const_iterator iter =
            m_niStatisticsUnits.find(niId);

    ASSERT(iter == m_niStatisticsUnits.end());

    std::pair<int,NIStatisticsUnit*> p =
            std::make_pair(niId, niSU);

    m_niStatisticsUnits.insert(p);
}

RouterStatisticsUnit* GlobalStatisticsUnit::getRouterStatisticsUnit(int routerId) {
    std::map<int,RouterStatisticsUnit*>::const_iterator iter =
            m_routerStatisticsUnits.find(routerId);

    ASSERT(iter != m_routerStatisticsUnits.end());

    return iter->second;
}

NIStatisticsUnit* GlobalStatisticsUnit::getNIStatisticsUnit(int niId) {
    std::map<int,NIStatisticsUnit*>::const_iterator iter =
            m_niStatisticsUnits.find(niId);

    ASSERT(iter != m_niStatisticsUnits.end());

    return iter->second;
}

void GlobalStatisticsUnit::initialize()
{
    // TODO - Generated method body
}

void GlobalStatisticsUnit::handleMessage(cMessage *msg)
{
    // should never happen
    ASSERT(false);
}

void GlobalStatisticsUnit::finish() {

}

GlobalStatisticsUnit* GlobalStatisticsUnit::s_getGlobalStatisticsUnit() {
    ASSERT(s_globalStatisticsUnit != NULL);
    return s_globalStatisticsUnit;
}

} //namespace
