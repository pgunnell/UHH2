#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/InclusiveJets/include/InclusiveJetsSelections.h"
#include "UHH2/InclusiveJets/include/InclusiveJetsHists.h"
#include "UHH2/InclusiveJets/include/MCWeightsModule.h"

#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/AdditionalSelections.h"
#include "UHH2/common/include/YearRunSwitchers.h"
#include "UHH2/common/include/JetCorrectionSets.h"
#include "UHH2/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "UHH2/JetMETObjects/interface/FactorizedJetCorrector.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {

/** \brief Basic analysis example of an AnalysisModule (formerly 'cycle') in UHH2
 * 
 * This is the central class which calls other AnalysisModules, Hists or Selection classes.
 * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
 */
class InclusiveJetsModule: public AnalysisModule {
public:
  
  explicit InclusiveJetsModule(Context & ctx);
  virtual bool process(Event & event) override;

private:
    
  std::unique_ptr<CommonModules> common;
  
  std::unique_ptr<JetCleaner> jetcleaner;
  std::unique_ptr<TriggerSelection> Trigger1_selection,Trigger2_selection,Trigger3_selection,Trigger4_selection,Trigger5_selection,Trigger6_selection,Trigger7_selection,Trigger8_selection,Trigger9_selection,Trigger10_selection;
  std::unique_ptr<JetCorrector> jec_mc, jec_data;
  std::unique_ptr<GenericJetResolutionSmearer> jer;

  //jet lepton cleaner
  //std::unique_ptr<YearSwitcher> JLC_MC, JLC_data;
  //std::shared_ptr<RunSwitcher> JLC_switcher_16, JLC_switcher_17, JLC_switcher_18;

  bool is_mc;
  JERSmearing::SFtype1 JER_sf;

  // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor,
  // to avoid memory leaks.
  std::unique_ptr<Selection> incljet_sel, dijet_sel, inclgenjet_sel,incljetresolution_sel,tagprobe_sel;
    
  // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
  std::unique_ptr<Hists> h_incljet, h_dijet, h_inclgenjet, h_incljetresolution, h_trigger,h_tagprobe;
  std::unique_ptr<Hists> h_trigger_trig1, h_trigger_trig2, h_trigger_trig3, h_trigger_trig4, h_trigger_trig5, h_trigger_trig6, h_trigger_trig7, h_trigger_trig8, h_trigger_trig9, h_trigger_trig10;

  const std::string triggerName1 = "HLT_ZeroBias_v*"; 
  const std::string triggerName2 = "HLT_PFJet40_*";
  const std::string triggerName3 = "HLT_PFJet60_*";
  const std::string triggerName4 = "HLT_PFJet80_*";
  const std::string triggerName5 = "HLT_PFJet140_*";
  const std::string triggerName6 = "HLT_PFJet200_*";
  const std::string triggerName7 = "HLT_PFJet260_*";
  const std::string triggerName8 = "HLT_PFJet320_*";
  const std::string triggerName9 = "HLT_PFJet400_*";
  const std::string triggerName10 = "HLT_PFJet450_*";
  
};

  double getLeadingJetPt(const Event & event){

    assert(event.jets);
    if(event.jets->size() < 1) return -10;
    const auto & jet0 = event.jets->at(0);
    return jet0.pt();

  }

  int getTriggerIndex(const Event & event){

    double leadingjetpt = getLeadingJetPt(event);

    double ptTriggerRanges[10] = {40.,74.,114.,196.,245.,330.,395.,468.,507.,650.};

    if(leadingjetpt<ptTriggerRanges[0]) return 0;
    if(leadingjetpt>=ptTriggerRanges[0] && leadingjetpt<ptTriggerRanges[1]) return 1;
    if(leadingjetpt>=ptTriggerRanges[1] && leadingjetpt<ptTriggerRanges[2]) return 2;
    if(leadingjetpt>=ptTriggerRanges[2] && leadingjetpt<ptTriggerRanges[3]) return 3;
    if(leadingjetpt>=ptTriggerRanges[3] && leadingjetpt<ptTriggerRanges[4]) return 4;
    if(leadingjetpt>=ptTriggerRanges[4] && leadingjetpt<ptTriggerRanges[5]) return 5;
    if(leadingjetpt>=ptTriggerRanges[5] && leadingjetpt<ptTriggerRanges[6]) return 6;
    if(leadingjetpt>=ptTriggerRanges[6] && leadingjetpt<ptTriggerRanges[7]) return 7;
    if(leadingjetpt>=ptTriggerRanges[7] && leadingjetpt<ptTriggerRanges[8]) return 8;
    if(leadingjetpt>=ptTriggerRanges[8] && leadingjetpt<ptTriggerRanges[9]) return 9;
    if(leadingjetpt>=ptTriggerRanges[9]) return 10;

    return 0;

  }

  
  void getTrigPrescale(Event & event, std::string triggerName){
    
    Event::TriggerIndex trigindex;
    trigindex = event.get_trigger_index(triggerName);
    
    int trigger_prescale = event.trigger_prescale(trigindex);
    int trigger_prescaleL1min = event.trigger_prescaleL1min(trigindex);
    int trigger_prescaleL1max = event.trigger_prescaleL1max(trigindex);
    
    event.weight *= (trigger_prescale*(trigger_prescaleL1min+trigger_prescaleL1max)/2); 
  }
  
  InclusiveJetsModule::InclusiveJetsModule(Context & ctx){
    // In the constructor, the typical tasks are to initialize the
    // member variables, in particular the AnalysisModules such as
    // CommonModules or some cleaner module, Selections and Hists.
    // But you can do more and e.g. access the configuration, as shown below.
    
    double jet_pt_min  = 30.0;
    double jet_eta_max = 2.4;
    
    // If running in SFrame, the keys "dataset_version", "dataset_type", "dataset_lumi",
    // and "target_lumi" are set to the according values in the xml file. For CMSSW, these are
    // not set automatically, but can be set in the python config file.
    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
    }
    
    // 1. setup other modules. CommonModules and the JetCleaner:
    //here common modules commented
    common.reset(new CommonModules());

    JetId id_jet = AndId<Jet>(JetPFID(JetPFID::WP_TIGHT_PUPPI), PtEtaCut(jet_pt_min, jet_eta_max));

    is_mc = ctx.get("dataset_type") == "MC";
    
    // TODO: configure common here, e.g. by 
    // calling common->set_*_id or common->disable_*
    
    //I want to select the JEC and JER I like, that's why I disable them in the common modules
    common->disable_jersmear();
    common->disable_jec();
    common->disable_lumisel(); //the Ntuples have been already produced with the correct json file

    //this needs to change for different years
    jec_mc.reset(new JetCorrector(ctx, JERFiles::JECFilesMC("Fall17_17Nov2017", "8", "AK4PFchs")));
    jec_data.reset(new JetCorrector(ctx, JERFiles::JECFilesDATA("Fall17_17Nov2017", "6", "AK4PFchs", "B")));

    JER_sf = JERSmearing::SF_13TeV_Fall17_V3;
    jer.reset(new GenericJetResolutionSmearer(ctx,"jets", "genjets", JER_sf, "2017/Fall17_V3_MC_PtResolution_AK4PFchs.txt"));  

    common->switch_jetlepcleaner(true);
    common->set_jet_id(id_jet);

    common->init(ctx);

    //it is fine not to correct the met since we don't use the met
    //jet-lepton cleaner can be removed since we want to count also the leptons inside the jets
    
    jetcleaner.reset(new JetCleaner(ctx, 30.0, 5.2)); 
    
    // note that the JetCleaner is only kept for the sake of example;
    // instead of constructing a jetcleaner explicitly,
    // the cleaning can also be achieved with less code via CommonModules with:
    // common->set_jet_id(PtEtaCut(30.0, 2.4));
    // before the 'common->init(ctx)' line.

    // 2. set up selections
    incljet_sel.reset(new InclusiveJetSelection(50.)); // see InclusiveJetsSelections
    inclgenjet_sel.reset(new InclusiveGenJetSelection(50.)); // see InclusiveJetsSelections
    incljetresolution_sel.reset(new InclusiveDetGenJetSelection(20.)); // see InclusiveJetsSelections
    dijet_sel.reset(new DijetSelection()); // see InclusiveJetsSelections
    tagprobe_sel.reset(new TagProbeSelection()); // see InclusiveJetsSelections

    // 3. Set up Hists classes:
    h_incljet.reset(new InclusiveJetsHists(ctx, "incljet"));
    h_inclgenjet.reset(new InclusiveGenJetsHists(ctx, "inclgenjet"));
    h_dijet.reset(new InclusiveJetsHists(ctx, "Dijet"));
    h_incljetresolution.reset(new InclusiveJetsResolutionHists(ctx, "Resolutions"));
    h_trigger.reset(new InclusiveJetsHists(ctx, "trigger"));
    h_tagprobe.reset(new InclusiveTagProbeHists(ctx, "tagprobe"));

    h_trigger_trig1.reset(new InclusiveJetsHists(ctx, "ZeroBias"));
    h_trigger_trig2.reset(new InclusiveJetsHists(ctx, "Jet40"));
    h_trigger_trig3.reset(new InclusiveJetsHists(ctx, "Jet60"));
    h_trigger_trig4.reset(new InclusiveJetsHists(ctx, "Jet80"));
    h_trigger_trig5.reset(new InclusiveJetsHists(ctx, "Jet140"));
    h_trigger_trig6.reset(new InclusiveJetsHists(ctx, "Jet200"));
    h_trigger_trig7.reset(new InclusiveJetsHists(ctx, "Jet260"));
    h_trigger_trig8.reset(new InclusiveJetsHists(ctx, "Jet320"));
    h_trigger_trig9.reset(new InclusiveJetsHists(ctx, "Jet400"));
    h_trigger_trig10.reset(new InclusiveJetsHists(ctx, "Jet450"));

    //triggers
    if ( triggerName1 != "NULL") Trigger1_selection.reset(new TriggerSelection( triggerName1 ));
    if ( triggerName2 != "NULL") Trigger2_selection.reset(new TriggerSelection( triggerName2 ));
    if ( triggerName3 != "NULL") Trigger3_selection.reset(new TriggerSelection( triggerName3 ));
    if ( triggerName4 != "NULL") Trigger4_selection.reset(new TriggerSelection( triggerName4 ));
    if ( triggerName5 != "NULL") Trigger5_selection.reset(new TriggerSelection( triggerName5 ));
    if ( triggerName6 != "NULL") Trigger6_selection.reset(new TriggerSelection( triggerName6 ));
    if ( triggerName7 != "NULL") Trigger7_selection.reset(new TriggerSelection( triggerName7 ));
    if ( triggerName8 != "NULL") Trigger8_selection.reset(new TriggerSelection( triggerName8 ));
    if ( triggerName9 != "NULL") Trigger9_selection.reset(new TriggerSelection( triggerName9 ));
    if ( triggerName10 != "NULL") Trigger10_selection.reset(new TriggerSelection( triggerName10 ));

}


bool InclusiveJetsModule::process(Event & event) {
    // This is the main procedure, called for each event. Typically,
    // do some pre-processing by calling the modules' process method
    // of the modules constructed in the constructor (1).
    // Then, test whether the event passes some selection and -- if yes --
    // use it to fill the histograms (2).
    // Finally, decide whether or not to keep the event in the output (3);
    // this is controlled by the return value of this method: If it
    // returns true, the event is kept; if it returns false, the event
    // is thrown away.
  
  // 1. run all modules other modules.
    common->process(event);
    jetcleaner->process(event);
    jer->process(event);
    
    if (is_mc) {
      jec_mc->process(event);
      jer->process(event);
      jec_mc->correct_met(event);
    } else {
      jec_data->process(event);
      jec_data->correct_met(event);
    }

    sort_by_pt(*event.jets);

    // 2. test selections and fill histograms
        
    bool dijet_selection = dijet_sel->passes(event);
    if(dijet_selection){
      h_dijet->fill(event);
    }

    bool inclusivejet_selection = incljet_sel->passes(event);
    if(inclusivejet_selection){
      h_incljet->fill(event);
    }

    bool inclusivegenjet_selection = false;

    bool tagprobe_selection = tagprobe_sel->passes(event);

    if(is_mc) {
      inclusivegenjet_selection = inclgenjet_sel->passes(event);
      if(inclusivegenjet_selection){
	h_inclgenjet->fill(event);
      }
    
      bool inclusivejetres_selection = incljetresolution_sel->passes(event);
      if(inclusivejetres_selection){
	h_incljetresolution->fill(event);
      }
    }
    
    bool trigger_selection = true;
    
    if(!is_mc) {
      int trig_index = getTriggerIndex(event); 

      //histograms from separate triggers without any prescales
      if(Trigger1_selection->passes(event)) h_trigger_trig1->fill(event);
      if(Trigger2_selection->passes(event)) h_trigger_trig2->fill(event);
      if(Trigger3_selection->passes(event)) h_trigger_trig3->fill(event);
      if(Trigger4_selection->passes(event)) h_trigger_trig4->fill(event);
      if(Trigger5_selection->passes(event)) h_trigger_trig5->fill(event);
      if(Trigger6_selection->passes(event)) h_trigger_trig6->fill(event);
      if(Trigger7_selection->passes(event)) h_trigger_trig7->fill(event);
      if(Trigger8_selection->passes(event)) h_trigger_trig8->fill(event);
      if(Trigger9_selection->passes(event)) h_trigger_trig9->fill(event);
      if(Trigger10_selection->passes(event)) h_trigger_trig10->fill(event);
          
      switch(trig_index){
      case 0: {trigger_selection = false; break;}
      case 1: {trigger_selection = Trigger1_selection->passes(event); getTrigPrescale(event,triggerName1); break;}
      case 2: {trigger_selection = Trigger2_selection->passes(event); getTrigPrescale(event,triggerName2); break;}
      case 3: {trigger_selection = Trigger3_selection->passes(event); getTrigPrescale(event,triggerName3); break;}
      case 4: {trigger_selection = Trigger4_selection->passes(event); getTrigPrescale(event,triggerName4); break;}
      case 5: {trigger_selection = Trigger5_selection->passes(event); getTrigPrescale(event,triggerName5); break;}
      case 6: {trigger_selection = Trigger6_selection->passes(event); getTrigPrescale(event,triggerName6); break;}
      case 7: {trigger_selection = Trigger7_selection->passes(event); getTrigPrescale(event,triggerName7); break;}
      case 8: {trigger_selection = Trigger8_selection->passes(event); getTrigPrescale(event,triggerName8); break;}
      case 9: {trigger_selection = Trigger9_selection->passes(event); getTrigPrescale(event,triggerName9); break;}
      case 10: {trigger_selection = Trigger10_selection->passes(event); getTrigPrescale(event,triggerName10); break;}
	//default: cout<<"check trigger values"<<endl;
      }	


    }

    if(trigger_selection){
      h_trigger->fill(event);
    }

    if(tagprobe_selection){
      h_tagprobe->fill(event);
    }
    
    // 3. decide whether or not to keep the current event in the output:
    return (trigger_selection && (inclusivejet_selection || inclusivegenjet_selection));
}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the InclusiveJetsModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(InclusiveJetsModule)

}
