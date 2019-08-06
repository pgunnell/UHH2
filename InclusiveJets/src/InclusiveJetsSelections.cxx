#include "UHH2/InclusiveJets/include/InclusiveJetsSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;


DijetSelection::DijetSelection(float dphi_min_, float third_frac_max_): dphi_min(dphi_min_), third_frac_max(third_frac_max_){}
    
bool DijetSelection::passes(const Event & event){
    assert(event.jets); // if this fails, it probably means jets are not read in
    if(event.jets->size() < 2) return false;
    const auto & jet0 = event.jets->at(0);
    const auto & jet1 = event.jets->at(1);
    auto dphi = deltaPhi(jet0, jet1);
    if(dphi < dphi_min) return false;
    if(event.jets->size() == 2) return true;
    const auto & jet2 = event.jets->at(2);
    auto third_jet_frac = jet2.pt() / (0.5 * (jet0.pt() + jet1.pt()));
    return third_jet_frac < third_frac_max;

}


InclusiveJetSelection::InclusiveJetSelection(float pTmin_): pTmin(pTmin_){}
    
bool InclusiveJetSelection::passes(const Event & event){
    assert(event.jets); // if this fails, it probably means jets are not read in
    if(event.jets->size() < 1) return false;
    const auto & jet0 = event.jets->at(0);
    if(jet0.pt()<pTmin && fabs(jet0.eta())<5.2) return false;
    return true;
}

InclusiveGenJetSelection::InclusiveGenJetSelection(float pTmin_): pTmin(pTmin_){}
    
bool InclusiveGenJetSelection::passes(const Event & event){
    assert(event.genjets); // if this fails, it probably means jets are not read in
    if(event.genjets->size() < 1) return false;
    const auto & jet0 = event.genjets->at(0);
    if(jet0.pt()<pTmin && fabs(jet0.eta())<5.2) return false;
    return true;
}

InclusiveDetGenJetSelection::InclusiveDetGenJetSelection(float pTmin_): pTmin(pTmin_){}
    
bool InclusiveDetGenJetSelection::passes(const Event & event){
    assert(event.genjets); // if this fails, it probably means jets are not read in
    assert(event.jets); // if this fails, it probably means jets are not read in
    if(event.genjets->size() < 1) return false;
    if(event.jets->size() < 1) return false;
    const auto & genjet0 = event.genjets->at(0);
    if(genjet0.pt()<pTmin && fabs(genjet0.eta())<5.2) return false;

    const auto & jet0 = event.jets->at(0);
    if(jet0.pt()<pTmin && fabs(jet0.eta())<5.2) return false;
    return true;
}

TagProbeSelection::TagProbeSelection(float pTmin_): pTmin(pTmin_){}
    
bool TagProbeSelection::passes(const Event & event){
    assert(event.jets); // if this fails, it probably means jets are not read in
    if(event.jets->size() < 2) return false;

    const auto & jet0 = event.jets->at(0);
    const auto & jet1 = event.jets->at(1);
    if(jet0.pt()<pTmin && fabs(jet0.eta())<5.2) return false;
    if(jet1.pt()<pTmin && fabs(jet1.eta())<5.2) return false;

    //deltaphi and relative pT of the third jet
    if(deltaPhi(jet0,jet1)<2.5) return false;
    if(event.jets->size() > 2){
      const auto & jet2 = event.jets->at(2);
      if(jet2.pt()>(jet1.pt()/2)) return false;
    }

    return true;
}

