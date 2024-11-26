#include "simulator/simulator.h"

namespace mapf::simulator {

Simulator::Simulator() : context_{std::make_shared<Context>()} {}

Simulator Simulator::FromConfig() { return Simulator(); }

Simulator& Simulator::Simulate() {
    while (!context_->event_queue->Empty()) {
        auto event = context_->event_queue->Extract();
        context_->current_time = event->timestamp;
        auto target_actor_wp = event->target_actor;
        if (auto target_actor = target_actor_wp.lock()) {
            target_actor->ReceiveMessage(std::move(event->message));
        }
    }
    return *this;
}

}  // namespace mapf::simulator