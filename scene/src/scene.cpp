#include "scene/scene.h"

#include "scene/proto/scene.pb.h"

#include <utility>

namespace mapf::scene {

Scene::Scene(models::AgentStates agent_states) : agent_states{std::move(agent_states)} {}

std::string Scene::SerializeToString() const {
    proto::Scene scene_proto;
    for (const auto& [agent_id, agent_state] : agent_states) {
        auto* agent_state_ptr = scene_proto.add_agent_state();
        agent_state_ptr->set_agent_id(agent_id);
        agent_state_ptr->set_node_id(agent_state.node_id);
    }
    std::string serialized_scene;
    scene_proto.SerializeToString(&serialized_scene);
    return serialized_scene;
}

}  // namespace mapf::scene