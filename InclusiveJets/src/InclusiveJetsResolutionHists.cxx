#include "UHH2/InclusiveJets/include/InclusiveJetsHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

InclusiveJetsResolutionHists::InclusiveJetsResolutionHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "N_{jets}", 20, 0, 20);  
  book<TH1F>("N_genjets", "N_{genjets}", 20, 0, 20);  

  const char* filename_hist="pt_resolution_";
  const char* filename_matr="pt_respmatrix_";

  double pTranges[81]={0., 1., 5., 6., 8., 10., 12., 15., 18., 21., 24., 28., 32., 37., 43., 49., 56., 64., 74., 84.,97., 114., 133., 153., 174., 196., 220., 245., 272., 300., 330., 362., 395., 430., 468.,507., 548., 592., 638., 686., 737., 790., 846., 905., 967.,1032., 1101., 1172., 1248., 1327., 1410., 1497., 1588., 1684., 1784., 1890., 2000.,2116., 2238., 2366., 2500., 2640., 2787., 2941., 3103., 3273., 3450., 3637., 3832.,4037., 4252., 4477., 4713., 4961., 5220., 5492.,5777., 6076., 6389., 6717., 7000};

  int Ptbins=80;
  
  for(int bin=0;bin<81;bin++){

    string ret;
    string ret_res;
    ret.append(filename_hist);
    ret_res.append(filename_matr);
    char bin_string[32];

    sprintf(bin_string, "%d", bin);
    ret.append(bin_string); 
    ret_res.append(bin_string); 

    book<TH1F>(ret, "#sigma(pT)/pT", 300, -1.5, 1.5);
  }

  book<TH2F>("ret_response_bin1", "Gen pT vs Det pT", Ptbins, pTranges,Ptbins, pTranges );
  book<TH2F>("ret_response_bin2", "Gen pT vs Det pT", Ptbins, pTranges,Ptbins, pTranges );
  book<TH2F>("ret_response_bin3", "Gen pT vs Det pT", Ptbins, pTranges,Ptbins, pTranges );
  book<TH2F>("ret_response_bin4", "Gen pT vs Det pT", Ptbins, pTranges,Ptbins, pTranges );
  book<TH2F>("ret_response_bin5", "Gen pT vs Det pT", Ptbins, pTranges,Ptbins, pTranges );
  book<TH2F>("ret_response_bin6", "Gen pT vs Det pT", Ptbins, pTranges,Ptbins, pTranges );
  book<TH2F>("ret_response_bin7", "Gen pT vs Det pT", Ptbins, pTranges,Ptbins, pTranges );

}


void InclusiveJetsResolutionHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;

  double pTranges[81]={0., 1., 5., 6., 8., 10., 12., 15., 18., 21., 24., 28., 32., 37., 43., 49., 56., 64., 74., 84.,97., 114., 133., 153., 174., 196., 220., 245., 272., 300., 330., 362., 395., 430., 468.,507., 548., 592., 638., 686., 737., 790., 846., 905., 967.,1032., 1101., 1172., 1248., 1327., 1410., 1497., 1588., 1684., 1784., 1890., 2000.,2116., 2238., 2366., 2500., 2640., 2787., 2941., 3103., 3273., 3450., 3637., 3832.,4037., 4252., 4477., 4713., 4961., 5220., 5492.,5777., 6076., 6389., 6717., 7000};
  
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();

  std::vector<GenJet>* genjets = event.genjets;
  int Ngenjets = genjets->size();

  const char* filename_hist = "pt_resolution_";

  for(int i=0;i<Njets;i++){
    for(int l=0;l<Ngenjets;l++){
      if(deltaR(jets->at(i),genjets->at(l))<0.4){
	
	if(fabs(jets->at(i).eta())<0.5) {((TH2F*)hist("ret_response_bin1"))->Fill(jets->at(i).pt(),genjets->at(l).pt(),weight); 	break;}
	if(fabs(jets->at(i).eta())>=0.5 && fabs(jets->at(i).eta())<1.0) {((TH2F*)hist("ret_response_bin2"))->Fill(jets->at(i).pt(),genjets->at(l).pt(),weight); 	break;}
	if(fabs(jets->at(i).eta())>=1.0 && fabs(jets->at(i).eta())<1.5) {((TH2F*)hist("ret_response_bin3"))->Fill(jets->at(i).pt(),genjets->at(l).pt(),weight); 	break;}
	if(fabs(jets->at(i).eta())>=1.5 && fabs(jets->at(i).eta())<2.0) {((TH2F*)hist("ret_response_bin4"))->Fill(jets->at(i).pt(),genjets->at(l).pt(),weight); 	break;}
	if(fabs(jets->at(i).eta())>=2.0 && fabs(jets->at(i).eta())<2.5) {((TH2F*)hist("ret_response_bin5"))->Fill(jets->at(i).pt(),genjets->at(l).pt(),weight); 	break;}
	if(fabs(jets->at(i).eta())>=2.5 && fabs(jets->at(i).eta())<3.0) {((TH2F*)hist("ret_response_bin6"))->Fill(jets->at(i).pt(),genjets->at(l).pt(),weight); 	break;}
	if(fabs(jets->at(i).eta())>=3.2 && fabs(jets->at(i).eta())<4.7) {((TH2F*)hist("ret_response_bin7"))->Fill(jets->at(i).pt(),genjets->at(l).pt(),weight); 	break;}
			    
	double resolution = (jets->at(i).pt()-genjets->at(l).pt())/genjets->at(l).pt();
	for(int j=0;j<81;j++){
	  if(genjets->at(l).pt()<pTranges[j+1] && genjets->at(l).pt()>pTranges[j]){
	    
	    string ret;
	    ret.append(filename_hist); 
	    char bin_string[32];
	    sprintf(bin_string, "%d", j);
	    ret.append(bin_string); 

	    const char *name = ret.c_str();
	    hist(name)->Fill(resolution,weight);
	    break;
	  }
	}
      }
      
    }
  }


  
}

InclusiveJetsResolutionHists::~InclusiveJetsResolutionHists(){}
