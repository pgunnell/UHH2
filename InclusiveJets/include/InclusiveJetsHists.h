#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/FlavorParticle.h"

namespace uhh2examples {

/**  \brief Example class for booking and filling histograms
 * 
 * NOTE: This class uses the 'hist' method to retrieve histograms.
 * This requires a string lookup and is therefore slow if you have
 * many histograms. Therefore, it is recommended to use histogram
 * pointers as member data instead, like in 'common/include/ElectronHists.h'.
 */
class InclusiveJetsHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    InclusiveJetsHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~InclusiveJetsHists();
};

class InclusiveGenJetsHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    InclusiveGenJetsHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~InclusiveGenJetsHists();
};

class InclusiveJetsResolutionHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    InclusiveJetsResolutionHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~InclusiveJetsResolutionHists();
};

class InclusiveTagProbeHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    InclusiveTagProbeHists(uhh2::Context & ctx, const std::string & dirname);
    uhh2::GenericEvent::Handle<std::vector<FlavorParticle>> handle_triggers[15]; 
    
    virtual void fill(const uhh2::Event & ev) override;
    virtual ~InclusiveTagProbeHists();
};

}
