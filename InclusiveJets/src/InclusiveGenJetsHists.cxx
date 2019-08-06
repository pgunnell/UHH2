#include "UHH2/InclusiveJets/include/InclusiveGenJetsHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

InclusiveGenJetsHists::InclusiveGenJetsHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_genjets", "N_{genjets}", 20, 0, 20);  
  book<TH1F>("eta_genjet1", "#eta^{genjet 1}", 40, -2.5, 2.5);
  book<TH1F>("eta_genjet2", "#eta^{genjet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_genjet3", "#eta^{genjet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_genjet4", "#eta^{genjet 4}", 40, -2.5, 2.5);

  //Pt binning                                                                                                                                                                                             
  double Ptbinning[81] = {0, 1, 5, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84,97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,507, 548, 592, 638, 686, 737, 790, 846, 905, 967,1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000,2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832,4037, 4252, 4477, 4713, 4961, 5220, 5492,5777, 6076, 6389, 6717, 7000};

  int Ptbins=80;

  book<TH1F>("pt_genjet1", "#pt^{genjet 1}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet2", "#pt^{genjet 2}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet3", "#pt^{genjet 3}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet4", "#pt^{genjet 4}", Ptbins,Ptbinning);

  // leptons
  book<TH1F>("N_mu", "N^{#mu}", 10, 0, 10);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, 0, 200);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -2.1, 2.1);
  book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);

  // primary vertices
  book<TH1F>("N_pv", "N^{PV}", 50, 0, 50);

  // inclusive jets

  book<TH1F>("pt_genjet_rap_bin1", "#pt^{genjet_rap_bin 1}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet_rap_bin2", "#pt^{genjet_rap_bin 2}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet_rap_bin3", "#pt^{genjet_rap_bin 3}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet_rap_bin4", "#pt^{genjet_rap_bin 4}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet_rap_bin5", "#pt^{genjet_rap_bin 5}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet_rap_bin6", "#pt^{genjet_rap_bin 6}", Ptbins,Ptbinning);
  book<TH1F>("pt_genjet_rap_bin7", "#pt^{genjet_rap_bin 7}", Ptbins,Ptbinning);

}


void InclusiveGenJetsHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;
  
  std::vector<GenJet>* genjets = event.genjets;
  int Ngenjets = genjets->size();
  hist("N_genjets")->Fill(Ngenjets, weight);
  
  if(Ngenjets>=1){
    hist("eta_genjet1")->Fill(genjets->at(0).eta(), weight);
  }
  if(Ngenjets>=2){
    hist("eta_genjet2")->Fill(genjets->at(1).eta(), weight);
  }
  if(Ngenjets>=3){
    hist("eta_genjet3")->Fill(genjets->at(2).eta(), weight);
  }
  if(Ngenjets>=4){
    hist("eta_genjet4")->Fill(genjets->at(3).eta(), weight);
  }

  for(int i=0;i<Ngenjets;i++){

    if(fabs(genjets->at(i).v4().Rapidity())<0.5) hist("pt_genjet_rap_bin1")->Fill(genjets->at(i).pt(), weight);
    if(fabs(genjets->at(i).v4().Rapidity())<1.0 && fabs(genjets->at(i).v4().Rapidity()) >= 0.5) hist("pt_genjet_rap_bin2")->Fill(genjets->at(i).pt(), weight);
    if(fabs(genjets->at(i).v4().Rapidity())<1.5 && fabs(genjets->at(i).v4().Rapidity()) >= 1.0) hist("pt_genjet_rap_bin3")->Fill(genjets->at(i).pt(), weight);
    if(fabs(genjets->at(i).v4().Rapidity())<2.0 && fabs(genjets->at(i).v4().Rapidity()) >= 1.5) hist("pt_genjet_rap_bin4")->Fill(genjets->at(i).pt(), weight);
    if(fabs(genjets->at(i).v4().Rapidity())<2.5 && fabs(genjets->at(i).v4().Rapidity()) >= 2.0) hist("pt_genjet_rap_bin5")->Fill(genjets->at(i).pt(), weight);
    if(fabs(genjets->at(i).v4().Rapidity())<3.0 && fabs(genjets->at(i).v4().Rapidity()) >= 2.5) hist("pt_genjet_rap_bin6")->Fill(genjets->at(i).pt(), weight);
    if(fabs(genjets->at(i).v4().Rapidity())<4.7 && fabs(genjets->at(i).v4().Rapidity()) >= 3.2) hist("pt_genjet_rap_bin7")->Fill(genjets->at(i).pt(), weight);
  }


  int Npvs = event.pvs->size();
  hist("N_pv")->Fill(Npvs, weight);
}

InclusiveGenJetsHists::~InclusiveGenJetsHists(){}
