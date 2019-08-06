#include "UHH2/InclusiveJets/include/InclusiveJetsHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

InclusiveTagProbeHists::InclusiveTagProbeHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets

  handle_triggers[0]= ctx.declare_event_input< vector< FlavorParticle > >(  "triggerObjects_hltDiPFJetAve40" );

  //Pt binning                                                                                                                                                                                             
  double Ptbinning[81] = {0, 1, 5, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84,97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,507, 548, 592, 638, 686, 737, 790, 846, 905, 967,1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000,2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832,4037, 4252, 4477, 4713, 4961, 5220, 5492,5777, 6076, 6389, 6717, 7000};

  int Ptbins=80;

  book<TH1F>("inclusive_pt_denominator", "Denominator for efficiency", Ptbins,Ptbinning);
  book<TH1F>("inclusive_pt_numerator", "Numerator For efficiency", Ptbins,Ptbinning);
  
}


void InclusiveTagProbeHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;
  
  std::vector<FlavorParticle> trig_obj_1 = event.get(handle_triggers[0]);

  const auto & jet0 = event.jets->at(0);
  const auto & jet1 = event.jets->at(1);

  double triggerThreshold = 40.;

  if(trig_obj_1.size() > 1){
    
    bool match = true;
    for(unsigned int l=0; l<trig_obj_1.size();l++){
      if(deltaR(trig_obj_1.at(l),jet0)<0.4 && trig_obj_1.at(l).pt()>triggerThreshold){ //matching with the leading jet
	match=true;
      }
    }
    
    for(unsigned int l=0; l<trig_obj_1.size();l++){
      if(match && deltaR(trig_obj_1.at(l),jet1)<0.4){ //matching with the subleading jet
	hist("inclusive_pt_denominator")->Fill(jet1.pt(),weight);
	if(trig_obj_1.at(l).pt()>triggerThreshold) 	hist("inclusive_pt_numerator")->Fill(jet1.pt(),weight);
      }
    }
  }
}

InclusiveTagProbeHists::~InclusiveTagProbeHists(){}
