#pragma once

#include <memory>
#include <typeindex>

namespace mapf::simulator {

struct Message {
    virtual ~Message() = default;
};

using MessagePtr = std::shared_ptr<Message>;

}  // namespace mapf::simulator