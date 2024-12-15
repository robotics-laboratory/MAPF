#pragma once

#include "solvers/solver_base.h"

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace mapf::solver {

class SolverFactory {
  private:
    using SolverFactoryMap = std::unordered_map<std::string, std::function<SolverPtr()>>;

  public:
    SolverFactory();

    static SolverFactory& Instance();

    SolverPtr CreateSolver(const std::string& name) const;

    SolverFactoryMap::const_iterator find(const std::string& name) const;
    SolverFactoryMap::const_iterator cbegin() const;
    SolverFactoryMap::const_iterator cend() const;

  private:
    SolverFactoryMap factory_map_;
};

}  // namespace mapf::solver