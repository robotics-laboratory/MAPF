#pragma once

#include "actors/actor.h"

#include "event/event_queue.h"

#include "context/context.h"

#include <memory>
#include <vector>

namespace mapf::simulator {

class Launcher {
  public:
    Launcher();

    template<typename ActorType, typename... Args>
    std::shared_ptr<ActorType> CreateActor(Args&&... args) {
        auto actor = std::make_shared<ActorType>(context_, std::forward<Args>(args)...);
        actors_.emplace_back(actor);
        return actor;
    }

    Launcher& Run();

  private:
    std::vector<ActorPtr> actors_;
    EventQueuePtr event_queue_;
    ContextPtr context_;
};

}  // namespace mapf::simulator