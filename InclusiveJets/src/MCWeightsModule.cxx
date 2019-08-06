#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Utils.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/InclusiveJets/include/MCWeightsModule.h"

#include <stdexcept>

using namespace uhh2;
using namespace std;

namespace uhh2examples{

MCScaleWeight::MCScaleWeight(Context & ctx){

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(!is_mc){
    cout << "Warning: MCScaleWeight will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  } else {

    auto s_mu_r = ctx.get("ScaleVariationMuR");
    auto s_mu_f = ctx.get("ScaleVariationMuF");
    
    if(s_mu_r == "up") {i_mu_r = 1;}
    else if(s_mu_r == "down"){i_mu_r = 2;}
    
    if(s_mu_f == "up"){i_mu_f = 1;}
    else if(s_mu_f == "down"){i_mu_f = 2;}
  }
}

bool MCScaleWeight::process(uhh2::Event & event){

  try{
    if(event.genInfo->systweights().size() == 0) return true; //no change in the weight, if weights are not saved

    if(i_mu_r == 0 && i_mu_f == 0) return true;
    else if(i_mu_r == 0 && i_mu_f == 1) syst_weight = event.genInfo->systweights().at(1);
    else if(i_mu_r == 0 && i_mu_f == 2) syst_weight = event.genInfo->systweights().at(2);
                                      
    else if(i_mu_r == 1 && i_mu_f == 0) syst_weight = event.genInfo->systweights().at(3);
    else if(i_mu_r == 1 && i_mu_f == 1) syst_weight = event.genInfo->systweights().at(4);
    else if(i_mu_r == 1 && i_mu_f == 2) syst_weight = event.genInfo->systweights().at(5);
                                      
    else if(i_mu_r == 2 && i_mu_f == 0) syst_weight = event.genInfo->systweights().at(6);
    else if(i_mu_r == 2 && i_mu_f == 1) syst_weight = event.genInfo->systweights().at(7);
    else if(i_mu_r == 2 && i_mu_f == 2) syst_weight = event.genInfo->systweights().at(8);

    event.weight *= syst_weight/event.genInfo->originalXWGTUP();
  }

  catch(const std::runtime_error& error){
    std::cout<<"Problem with genInfo in MCWeight.cxx"<<std::endl;
    std::cout<<error.what();
  }
  
  return true;
}

MCShowerWeight::MCShowerWeight(Context & ctx){

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(!is_mc){
    cout << "Warning: MCShowerWeight will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  } else {

    auto shower_scale = ctx.get("ShowerScaleVariation"); //change here
    auto shower_core = ctx.get("ShowerCoreVariation"); //change here
    
    if(shower_core == "down") {shower_core = 1;}
    if(shower_core == "up") {shower_core = 2;}

    if(shower_scale == "down"){shower_scale = 1;}
    if(shower_scale == "up"){shower_scale = 2;}
    
  }
}

bool MCShowerWeight::process(uhh2::Event & event){

  try{
    if(event.genInfo->systweights().size() == 0) return true; //no change in the weight, if weights are not saved

    if(shower_core == 0 && shower_scale == 0) return true;
    else if(shower_core == 0 && shower_scale == 1) syst_weight = event.genInfo->weights().at(1); //change here and following
    else if(shower_core == 0 && shower_scale == 2) syst_weight = event.genInfo->weights().at(2);
                                      
    else if(shower_core == 1 && shower_scale == 0) syst_weight = event.genInfo->weights().at(3);
    else if(shower_core == 1 && shower_scale == 1) syst_weight = event.genInfo->weights().at(4);
    else if(shower_core == 1 && shower_scale == 2) syst_weight = event.genInfo->weights().at(5);
                                      
    else if(shower_core == 2 && shower_scale == 0) syst_weight = event.genInfo->weights().at(6);
    else if(shower_core == 2 && shower_scale == 1) syst_weight = event.genInfo->weights().at(7);
    else if(shower_core == 2 && shower_scale == 2) syst_weight = event.genInfo->weights().at(8);

    event.weight *= syst_weight/event.genInfo->originalXWGTUP();
  }

  catch(const std::runtime_error& error){
    std::cout<<"Problem with genInfo in MCWeight.cxx"<<std::endl;
    std::cout<<error.what();
  }
  
  return true;
}

//PDF variations
  
MCPDFWeight::MCPDFWeight(Context & ctx){

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(!is_mc){
    cout << "Warning: MCPDFWeight will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  } else {
    
    auto pdf_var = ctx.get("PDFScaleVariation");
    
  }
}

bool MCPDFWeight::process(uhh2::Event & event){

  try{
    if(event.genInfo->systweights().size() == 0) return true; //no change in the weight, if weights are not saved

    if(pdf_var == 0) return true;
    if(pdf_var > 100) {
      cout<<" There is no PDF variation associated to values above 100; using nominal weight"<<endl;
      return true;
    }

    for(int i=1;i<101; i++){
      if(pdf_var==i){
	syst_weight = event.genInfo->systweights().at(8+i); // PDF weights start from position 9
      }
    }

    event.weight *= syst_weight/event.genInfo->originalXWGTUP();
  }

  catch(const std::runtime_error& error){
    std::cout<<"Problem with genInfo in MCWeight.cxx"<<std::endl;
    std::cout<<error.what();
  }
  
  return true;
}

}
