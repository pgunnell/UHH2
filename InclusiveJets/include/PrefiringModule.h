#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

namespace uhh2examples {
    
/* For handling prefiring weights */
  
  class PrefiringWeight: public uhh2::AnalysisModule {
  public:
    explicit PrefiringWeight(uhh2::Context & ctx);

    virtual bool process(uhh2::Event & event) override;

  private:
    int prefire_weight = 0;
    double syst_weight;
  };

}
