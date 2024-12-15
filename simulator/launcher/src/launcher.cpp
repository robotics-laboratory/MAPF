#include "launcher/launcher.h"

namespace mapf::simulator {

Launcher::Launcher() :
    actors_{},
    event_queue_{std::make_shared<EventQueue>()},
    context_{std::make_shared<Context>(std::make_shared<EventBus>(event_queue_))} {}

Launcher& Launcher::Run() {
    for (auto& actor : actors_) {
        actor->OnStart();
    }
    while (!event_queue_->Empty()) {
        auto event = event_queue_->Extract();
        context_->current_time = event->timestamp;
        event->action();
    }
    for (auto& actor : actors_) {
        actor->OnFinish();
    }
    return *this;
}

}  // namespace mapf::simulator