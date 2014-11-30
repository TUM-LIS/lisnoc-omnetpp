/*
 * FaultModelZimmerJantsch.h
 *
 *  Created on: Nov 28, 2014
 *      Author: gu45zin
 */

#ifndef FAULTMODELZIMMERJANTSCH_H_
#define FAULTMODELZIMMERJANTSCH_H_

#include <FaultModel.h>

#include <random>

namespace lisnoc {

class FaultModelZimmerJantsch: public lisnoc::FaultModelBase {
private:
    typedef int Pw_t;
    typedef int Pt_t;
    typedef std::pair<Pw_t, Pt_t> Pentry_t;
    typedef std::vector<Pentry_t> P_t;
    P_t m_P3;

    double m_p_faultLink;
    double m_p_faultBuffer;

    std::discrete_distribution<int64_t> *m_sampleFaultDistributionLink;
    std::discrete_distribution<int64_t> *m_sampleFaultDistributionBuffer;
    std::discrete_distribution<int64_t> *m_sampleCharacteristicDistribution;

protected:
    virtual void initialize(int stage);
public:
    virtual bool sampleFaultLink();
    virtual bool sampleFaultBuffer();
    virtual void sampleFaultCharacteristics(std::vector<struct FaultCharacteristics> &faultVector);
};

} /* namespace lisnoc */

#endif /* FAULTMODELZIMMERJANTSCH_H_ */
