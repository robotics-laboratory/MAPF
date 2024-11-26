#pragma once

#include <memory>
#include <typeindex>

namespace mapf::simulator {

struct Message {
    virtual ~Message() = default;

    virtual std::type_index type() const = 0;
};

using MessagePtr = std::shared_ptr<Message>;

}  // namespace mapf::simulator