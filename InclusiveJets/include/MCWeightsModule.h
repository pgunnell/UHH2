#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

namespace uhh2examples {
    
/* For handling MC weights */
  
  class MCScaleWeight: public uhh2::AnalysisModule {
  public:
    explicit MCScaleWeight(uhh2::Context & ctx);

    virtual bool process(uhh2::Event & event) override;

  private:
    double syst_weight;
    int i_mu_r = 0, i_mu_f = 0;
  };


  class MCShowerWeight: public uhh2::AnalysisModule {
  public:
    explicit MCShowerWeight(uhh2::Context & ctx);

    virtual bool process(uhh2::Event & event) override;

  private:
    double syst_weight;
    int shower_scale = 0, shower_core = 0;
  };


  class MCPDFWeight: public uhh2::AnalysisModule {
  public:
    explicit MCPDFWeight(uhh2::Context & ctx);

    virtual bool process(uhh2::Event & event) override;

  private:
    double syst_weight;
    int pdf_var = 0;
  };



}
