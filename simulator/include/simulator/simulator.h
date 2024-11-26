#pragma once

#include "simulator/arctor.h"
#include "simulator/context.h"

namespace mapf::simulator {

class Simulator {
  public:
    Simulator();

    static Simulator FromConfig(/*SimulatorConfig*/);

    Simulator& Simulate();

  private:
    ContextPtr context_;
};

}  // namespace mapf::simulator