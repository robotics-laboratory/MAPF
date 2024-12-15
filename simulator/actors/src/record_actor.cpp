#define MCAP_IMPLEMENTATION

#include "actors/record_actor.h"

#include "messages/request_agent_states_message.h"

#include "scene/scene.h"
#include "scene/proto/scene.pb.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <chrono>
#include <format>
#include <utility>

namespace {

google::protobuf::FileDescriptorSet BuildFileDescriptorSet(
    const google::protobuf::Descriptor* toplevelDescriptor) {
    google::protobuf::FileDescriptorSet fdSet;
    std::queue<const google::protobuf::FileDescriptor*> toAdd;
    toAdd.push(toplevelDescriptor->file());
    std::unordered_set<std::string> seenDependencies;
    while (!toAdd.empty()) {
        const google::protobuf::FileDescriptor* next = toAdd.front();
        toAdd.pop();
        next->CopyTo(fdSet.add_file());
        for (int i = 0; i < next->dependency_count(); ++i) {
            const auto& dep = next->dependency(i);
            if (seenDependencies.find(dep->name()) == seenDependencies.end()) {
                seenDependencies.insert(dep->name());
                toAdd.push(dep);
            }
        }
    }
    return fdSet;
}

}  // namespace

namespace mapf::simulator {

RecordActor::RecordActor(ContextPtr context, std::filesystem::path scene_file, double period) :
    Actor{std::move(context)}, scene_file_{std::move(scene_file)}, period_{period} {
    auto res = writer_.open(scene_file_.string(), mcap::McapWriterOptions(""));
    if (!res.ok()) {
        throw std::runtime_error(std::format("Failed to open file {}", scene_file_.string()));
    }
    mcap::Schema schema(
        "mapf.scene.proto.Scene",
        "protobuf",
        BuildFileDescriptorSet(scene::proto::Scene::descriptor()).SerializeAsString());
    writer_.addSchema(schema);
    channel_ = mcap::Channel{"scene_data", "protobuf", schema.id};
    writer_.addChannel(channel_);

    context_->event_bus->Subscribe<ResponseAgentStatesMessage>([this](MessagePtr message) {
        HandleResponseAgentStatesMessage(
            std::static_pointer_cast<ResponseAgentStatesMessage>(std::move(message)));
    });
}

RecordActor::~RecordActor() { writer_.close(); }

RecordActor& RecordActor::OnStart() {
    context_->event_bus->Publish(
        context_->current_time, std::make_shared<RequestAgentStatesMessage>());
    return *this;
}

void RecordActor::HandleResponseAgentStatesMessage(
    std::shared_ptr<ResponseAgentStatesMessage> response_agent_states_message) {
    auto scene = scene::Scene(std::move(*(response_agent_states_message->agent_states)));
    auto serialized = scene.SerializeToString();
    auto msg_ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
                      std::chrono::duration<double>(context_->current_time))
                      .count();
    mcap::Message msg;
    msg.channelId = channel_.id;
    msg.publishTime = msg_ts;
    msg.logTime = msg_ts;
    msg.data = reinterpret_cast<const std::byte*>(serialized.data());
    msg.dataSize = serialized.size();
    const auto res = writer_.write(msg);
    if (!res.ok()) {
        throw std::runtime_error("Failed to write MCAP");
    }
    if (!response_agent_states_message->all_finished) {
        context_->event_bus->Publish(
            context_->current_time + period_, std::make_shared<RequestAgentStatesMessage>());
    }
}

}  // namespace mapf::simulator