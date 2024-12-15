#pragma once

#include "actors/actor.h"

#include "messages/response_agent_states_message.h"

#include <mcap/writer.hpp>

#include <filesystem>
#include <memory>

namespace mapf::simulator {

class RecordActor final : public Actor {
  public:
    RecordActor(ContextPtr context, std::filesystem::path scene_file, double period);

    ~RecordActor() final;

    RecordActor& OnStart() final;

  private:
    void HandleResponseAgentStatesMessage(
        std::shared_ptr<ResponseAgentStatesMessage> response_agent_states_message);

    std::filesystem::path scene_file_;
    double period_;
    mcap::McapWriter writer_;
    mcap::Channel channel_;
};

}  // namespace mapf::simulator