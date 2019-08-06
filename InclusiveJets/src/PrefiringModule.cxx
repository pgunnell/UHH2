#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Utils.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/InclusiveJets/include/PrefiringModule.h"

#include <stdexcept>

using namespace uhh2;
using namespace std;

namespace uhh2examples{

PrefiringWeight::PrefiringWeight(Context & ctx){

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(is_mc){
    cout << "Warning: PrefiringWeight will not have an effect on this MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  } else {

    auto prefire_weight_choice = ctx.get("PrefiringWeight");
    
    if(prefire_weight_choice == "up") {prefire_weight = 1;}
    else if(prefire_weight_choice == "down") {prefire_weight = 2;}
    else if(prefire_weight_choice == "nominal") {prefire_weight = 0;}
    else prefire_weight = 3;
  }
}

bool PrefiringWeight::process(uhh2::Event & event){

  if(prefire_weight == 3) return true;
  else if(prefire_weight == 1) syst_weight = event.prefiringWeightUp;
  else if(prefire_weight == 2) syst_weight = event.prefiringWeightDown;
  else if(prefire_weight == 0) syst_weight = event.prefiringWeight;
  
  event.weight *= syst_weight;
  
  return true;
}

}
