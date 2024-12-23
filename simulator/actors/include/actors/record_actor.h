#pragma once

#include "actors/actor.h"

#include "foxglove/proto/foxglove.pb.h"

#include "graph/graph.h"

#include "messages/response_agent_states_message.h"

#include "models/models.h"

#include <mcap/writer.hpp>

#include <filesystem>
#include <memory>

namespace mapf::simulator {

class RecordActor final : public Actor {
  public:
    RecordActor(
        ContextPtr context, std::filesystem::path scene_file, double period, graph::GraphPtr graph);

    ~RecordActor() final;

    RecordActor& OnStart() final;

  private:
    void HandleResponseAgentStatesMessage(
        std::shared_ptr<ResponseAgentStatesMessage> response_agent_states_message);

    RecordActor& WriteGraph();

    RecordActor& WriteAgentStates(const models::AgentStates& agent_states);

    foxglove::SceneUpdate& ResetSceneUpdate();

    RecordActor& WriteSceneUpdate();

    std::filesystem::path scene_file_;
    double period_;
    graph::GraphPtr graph_;
    foxglove::SceneUpdate scene_update_;
    double scene_update_ts_;
    mcap::McapWriter writer_;
    mcap::Channel channel_;
};

}  // namespace mapf::simulator