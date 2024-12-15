#pragma once

#include "event/event_queue.h"

#include "messages/message.h"

#include <concepts>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace mapf::simulator {

using MessageHandler = std::function<void(MessagePtr)>;

class EventBus {
  public:
    EventBus(EventQueuePtr event_queue);

    template<std::derived_from<Message> MessageType>
    EventBus& Subscribe(MessageHandler handler) {
        subscribers_[std::type_index(typeid(MessageType))].emplace_back(std::move(handler));
        return *this;
    }

    template<std::derived_from<Message> MessageType>
    EventBus& Publish(double timestamp, std::shared_ptr<MessageType> message) {
        auto message_type_index = std::type_index(typeid(MessageType));
        if (!subscribers_.contains(message_type_index)) {
            return *this;
        }
        auto callback = [this, message_type_index, message] {
            for (const auto& handler : subscribers_.at(message_type_index)) {
                handler(message);
            }
        };
        event_queue_->Push(std::make_shared<Event>(timestamp, std::move(callback)));
        return *this;
    }

  private:
    EventQueuePtr event_queue_;
    std::unordered_map<std::type_index, std::vector<MessageHandler>> subscribers_;
};

using EventBusPtr = std::shared_ptr<EventBus>;

}  // namespace mapf::simulator