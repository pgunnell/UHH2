#include "UHH2/InclusiveJets/include/InclusiveJetsHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

InclusiveJetsHists::InclusiveJetsHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "N_{jets}", 20, 0, 20);  
  book<TH1F>("eta_jet1", "#eta^{jet 1}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet2", "#eta^{jet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{jet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet4", "#eta^{jet 4}", 40, -2.5, 2.5);

  //Pt binning                                                                                                                                                                                             
  double Ptbinning[81] = {0, 1, 5, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84,97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,507, 548, 592, 638, 686, 737, 790, 846, 905, 967,1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000,2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832,4037, 4252, 4477, 4713, 4961, 5220, 5492,5777, 6076, 6389, 6717, 7000};

  int Ptbins=80;

  book<TH1F>("pt_jet1", "#pt^{jet 1}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet2", "#pt^{jet 2}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet3", "#pt^{jet 3}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet4", "#pt^{jet 4}", Ptbins,Ptbinning);

  // leptons
  book<TH1F>("N_mu", "N^{#mu}", 10, 0, 10);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, 0, 200);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -2.1, 2.1);
  book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);

  // primary vertices
  book<TH1F>("N_pv", "N^{PV}", 50, 0, 50);

  // inclusive jets

  book<TH1F>("pt_jet_rap_bin1", "#pt^{jet_rap_bin 1}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet_rap_bin2", "#pt^{jet_rap_bin 2}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet_rap_bin3", "#pt^{jet_rap_bin 3}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet_rap_bin4", "#pt^{jet_rap_bin 4}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet_rap_bin5", "#pt^{jet_rap_bin 5}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet_rap_bin6", "#pt^{jet_rap_bin 6}", Ptbins,Ptbinning);
  book<TH1F>("pt_jet_rap_bin7", "#pt^{jet_rap_bin 7}", Ptbins,Ptbinning);

  //jet constituents
  book<TH1F>("chg_haden_fraction", "Charged hadron energy fraction", 100,0.,1.);
  book<TH1F>("neutral_haden_fraction", "Neutral hadron energy fraction",  100,0.,1.);

  book<TH1F>("chg_elmen_fraction", "Charged elm energy fraction", 100,0.,1.);
  book<TH1F>("neutral_elmen_fraction", "Neutral elm energy fraction",  100,0.,1.);

  book<TH1F>("photon_en_fraction", "Photon elm energy fraction",  100,0.,1.);
  book<TH1F>("muon_en_fraction", "Muon energy fraction",  100,0.,1.);

  book<TH1F>("chg_had_multiplicity", "Charged hadron energy multiplicity", 100,0.,100.);
  book<TH1F>("neutral_had_multiplicity", "Neutral hadron energy multiplicity", 100,0.,100.);

  book<TH1F>("electron_multiplicity", "Electron multiplicity", 100,0.,100.);

  book<TH1F>("muon_multiplicity", "Muon multiplicity", 100,0.,100.);
  book<TH1F>("photon_multiplicity", "Photon multiplicity", 100,0.,100.);

  //puppi multiplicities
  book<TH1F>("puppi_multiplicity", "Puppi multiplicity", 100,0.,100.);
  book<TH1F>("neutral_puppi_multiplicity", "Neutral puppi multiplicity", 100,0.,100.);
  book<TH1F>("neutral_had_puppi_multiplicity", "Neutral hadron puppi multiplicity", 100,0.,100.);
  book<TH1F>("photon_puppi_multiplicity", "Photon puppi multiplicity", 100,0.,100.);
  book<TH1F>("hf_puppi_multiplicity", "HF puppi multiplicity", 100,0.,100.);
  book<TH1F>("hfem_puppi_multiplicity", "HF em puppi multiplicity", 100,0.,100.);

}


void InclusiveJetsHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;
  
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();
  hist("N_jets")->Fill(Njets, weight);
  
  if(Njets>=1){
    hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
    hist("pt_jet1")->Fill(jets->at(0).pt(), weight);
  }
  if(Njets>=2){
    hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
    hist("pt_jet2")->Fill(jets->at(1).pt(), weight);
  }
  if(Njets>=3){
    hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
    hist("pt_jet3")->Fill(jets->at(2).pt(), weight);
  }
  if(Njets>=4){
    hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
    hist("pt_jet4")->Fill(jets->at(3).pt(), weight);
  }

  for(int i=0;i<Njets;i++){

    if(fabs(jets->at(i).v4().Rapidity())<0.5) hist("pt_jet_rap_bin1")->Fill(jets->at(i).pt(), weight);
    if(fabs(jets->at(i).v4().Rapidity())<1.0 && fabs(jets->at(i).v4().Rapidity()) >= 0.5) hist("pt_jet_rap_bin2")->Fill(jets->at(i).pt(), weight);
    if(fabs(jets->at(i).v4().Rapidity())<1.5 && fabs(jets->at(i).v4().Rapidity()) >= 1.0) hist("pt_jet_rap_bin3")->Fill(jets->at(i).pt(), weight);
    if(fabs(jets->at(i).v4().Rapidity())<2.0 && fabs(jets->at(i).v4().Rapidity()) >= 1.5) hist("pt_jet_rap_bin4")->Fill(jets->at(i).pt(), weight);
    if(fabs(jets->at(i).v4().Rapidity())<2.5 && fabs(jets->at(i).v4().Rapidity()) >= 2.0) hist("pt_jet_rap_bin5")->Fill(jets->at(i).pt(), weight);
    if(fabs(jets->at(i).v4().Rapidity())<3.0 && fabs(jets->at(i).v4().Rapidity()) >= 2.5) hist("pt_jet_rap_bin6")->Fill(jets->at(i).pt(), weight);
    if(fabs(jets->at(i).v4().Rapidity())<4.7 && fabs(jets->at(i).v4().Rapidity()) >= 3.2) hist("pt_jet_rap_bin7")->Fill(jets->at(i).pt(), weight);

    //perhaps one has to separate them in rapidity ranges
    if(fabs(jets->at(i).v4().Rapidity())<0.5 && jets->at(i).pt()>50) continue;
    hist("chg_haden_fraction")->Fill(jets->at(i).chargedHadronEnergyFraction(),weight);
    hist("neutral_haden_fraction")->Fill(jets->at(i).chargedHadronEnergyFraction(),weight);
    hist("chg_elmen_fraction")->Fill(jets->at(i).chargedEmEnergyFraction(),weight);
    hist("neutral_elmen_fraction")->Fill(jets->at(i).chargedHadronEnergyFraction(),weight);
    hist("photon_en_fraction")->Fill(jets->at(i).photonEnergyFraction(),weight);
    hist("muon_en_fraction")->Fill(jets->at(i).muonEnergyFraction(),weight);
    hist("photon_multiplicity")->Fill(jets->at(i).photonMultiplicity(),weight);
    hist("electron_multiplicity")->Fill(jets->at(i).electronMultiplicity(),weight);
    hist("muon_multiplicity")->Fill(jets->at(i).muonMultiplicity(),weight);
    hist("neutral_had_multiplicity")->Fill(jets->at(i).neutralMultiplicity(),weight);
    hist("chg_had_multiplicity")->Fill(jets->at(i).chargedMultiplicity(),weight);
    hist("puppi_multiplicity")->Fill(jets->at(i).puppiMultiplicity(),weight);
    hist("neutral_puppi_multiplicity")->Fill(jets->at(i).neutralPuppiMultiplicity(),weight);
    hist("neutral_had_puppi_multiplicity")->Fill(jets->at(i).neutralHadronPuppiMultiplicity(),weight);
    hist("photon_puppi_multiplicity")->Fill(jets->at(i).photonPuppiMultiplicity(),weight);
    hist("hf_puppi_multiplicity")->Fill(jets->at(i).HFHadronPuppiMultiplicity(),weight);
    hist("hfem_puppi_multiplicity")->Fill(jets->at(i).HFEMPuppiMultiplicity(),weight);

  }


  int Npvs = event.pvs->size();
  hist("N_pv")->Fill(Npvs, weight);
}

InclusiveJetsHists::~InclusiveJetsHists(){}
