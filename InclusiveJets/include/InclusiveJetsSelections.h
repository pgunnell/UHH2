#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"
#include "UHH2/core/include/FlavorParticle.h"

namespace uhh2examples {
    
/* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
 * below 20% of the average of the leading two jets, where the minimum deltaphi and
 * maximum third jet pt fraction can be changed in the constructor.
 * The jets are assumed to be sorted in pt.
 */
class DijetSelection: public uhh2::Selection {
public:
    DijetSelection(float dphi_min = 2.7f, float third_frac_max = 0.2f);
    virtual bool passes(const uhh2::Event & event) override;
private:
    float dphi_min, third_frac_max; 
};

class InclusiveJetSelection: public uhh2::Selection {
public:
    InclusiveJetSelection(float pTmin = 100.);
    virtual bool passes(const uhh2::Event & event) override;
private:
    float pTmin; 
};


class InclusiveGenJetSelection: public uhh2::Selection {
public:
    InclusiveGenJetSelection(float pTmin = 100.);
    virtual bool passes(const uhh2::Event & event) override;
private:
    float pTmin; 
};

class InclusiveDetGenJetSelection: public uhh2::Selection {
public:
    InclusiveDetGenJetSelection(float pTmin = 100.);
    virtual bool passes(const uhh2::Event & event) override;
private:
    float pTmin; 
};

class TagProbeSelection: public uhh2::Selection {
public:
    TagProbeSelection(float pTmin = 100.);
    virtual bool passes(const uhh2::Event & event) override;
private:
    float pTmin; 
};

}
