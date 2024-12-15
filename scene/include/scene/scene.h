#pragma once

#include "models/models.h"

#include <memory>
#include <string>

namespace mapf::scene {

struct Scene {
    Scene() = default;

    Scene(models::AgentStates agent_states);

    std::string SerializeToString() const;

    models::AgentStates agent_states;
};

using ScenePtr = std::shared_ptr<Scene>;

}  // namespace mapf::scene