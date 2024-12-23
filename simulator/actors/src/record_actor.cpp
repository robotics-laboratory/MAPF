#define MCAP_IMPLEMENTATION

#include "actors/record_actor.h"

#include "geom/common.h"

#include "messages/request_agent_states_message.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <chrono>
#include <format>
#include <utility>

namespace {

struct RGB {
    float r, g, b;
};

RGB hsv2rgb(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h >= 60 && h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h >= 180 && h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h >= 240 && h < 300) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }

    return RGB{.r = r + m, .g = g + m, .b = b + m};
}

google::protobuf::FileDescriptorSet BuildFileDescriptorSet(
    const google::protobuf::Descriptor* top_level_descriptor) {
    google::protobuf::FileDescriptorSet fd_set;
    std::queue<const google::protobuf::FileDescriptor*> to_add;
    to_add.push(top_level_descriptor->file());
    std::unordered_set<std::string> seen_dependencies;
    while (!to_add.empty()) {
        const google::protobuf::FileDescriptor* next = to_add.front();
        to_add.pop();
        next->CopyTo(fd_set.add_file());
        for (int i = 0; i < next->dependency_count(); ++i) {
            const auto& dep = next->dependency(i);
            if (seen_dependencies.find(dep->name()) == seen_dependencies.end()) {
                seen_dependencies.insert(dep->name());
                to_add.push(dep);
            }
        }
    }
    return fd_set;
}

}  // namespace

namespace mapf::simulator {

RecordActor::RecordActor(
    ContextPtr context, std::filesystem::path scene_file, double period, graph::GraphPtr graph) :
    Actor{std::move(context)},
    scene_file_{std::move(scene_file)},
    period_{period},
    graph_{std::move(graph)},
    scene_update_{},
    scene_update_ts_{context_->current_time} {
    auto res = writer_.open(scene_file_.string(), mcap::McapWriterOptions(""));
    if (!res.ok()) {
        throw std::runtime_error(std::format("Failed to open file {}", scene_file_.string()));
    }
    mcap::Schema schema(
        "foxglove.SceneUpdate",
        "protobuf",
        BuildFileDescriptorSet(foxglove::SceneUpdate::descriptor()).SerializeAsString());
    writer_.addSchema(schema);
    channel_ = mcap::Channel(
        "/foxglove/SceneUpdate",
        "protobuf",
        schema.id,
        {{"protobuf_message_type", "foxglove.SceneUpdate"}});
    writer_.addChannel(channel_);

    context_->event_bus->Subscribe<ResponseAgentStatesMessage>([this](MessagePtr message) {
        HandleResponseAgentStatesMessage(
            std::static_pointer_cast<ResponseAgentStatesMessage>(std::move(message)));
    });
}

RecordActor::~RecordActor() {
    WriteSceneUpdate();
    writer_.close();
}

RecordActor& RecordActor::OnStart() {
    WriteGraph();
    context_->event_bus->Publish(
        context_->current_time, std::make_shared<RequestAgentStatesMessage>());
    return *this;
}

void RecordActor::HandleResponseAgentStatesMessage(
    std::shared_ptr<ResponseAgentStatesMessage> response_agent_states_message) {
    if (response_agent_states_message->agent_states == nullptr) {
        return;
    }
    WriteAgentStates(*(response_agent_states_message->agent_states));
    if (!response_agent_states_message->all_finished) {
        context_->event_bus->Publish(
            context_->current_time + period_, std::make_shared<RequestAgentStatesMessage>());
    }
}

RecordActor& RecordActor::WriteGraph() {
    auto& scene_update = ResetSceneUpdate();

    auto* node_entity = scene_update.add_entities();
    node_entity->set_frame_id("scene");
    node_entity->set_id("nodes");
    node_entity->set_frame_locked(true);

    auto* lifetime = node_entity->mutable_lifetime();
    lifetime->set_seconds(0);
    lifetime->set_nanos(0);

    for (const auto& [_, node] : graph_->nodes) {
        {
            auto* cylinder = node_entity->add_cylinders();

            auto* pose = cylinder->mutable_pose();
            auto* position = pose->mutable_position();
            position->set_x(node.pos.x);
            position->set_y(node.pos.y);
            position->set_z(0.0);

            auto* size = cylinder->mutable_size();
            size->set_x(0.2);
            size->set_y(0.2);
            size->set_z(0.05);

            auto* color = cylinder->mutable_color();
            color->set_r(0.7);
            color->set_g(0.7);
            color->set_b(0.7);
            color->set_a(1.0);
        }

        {
            auto* text = node_entity->add_texts();
            text->set_text("Node " + std::to_string(node.id));
            text->set_billboard(true);
            text->set_font_size(12);
            text->set_scale_invariant(true);

            auto* color = text->mutable_color();
            color->set_r(255);
            color->set_g(255);
            color->set_b(255);
            color->set_a(1.0);

            auto* text_pose = text->mutable_pose();
            auto* text_pos = text_pose->mutable_position();
            text_pos->set_x(node.pos.x);
            text_pos->set_y(node.pos.y);
            text_pos->set_z(0.06);
        }
    }

    auto* edge_entity = scene_update.add_entities();
    edge_entity->set_frame_id("scene");
    edge_entity->set_id("edges");
    edge_entity->set_frame_locked(true);

    lifetime = edge_entity->mutable_lifetime();
    lifetime->set_seconds(0);
    lifetime->set_nanos(0);

    auto* lines = edge_entity->add_lines();
    lines->set_type(foxglove::LinePrimitive::LINE_LIST);
    lines->set_thickness(0.05);

    auto* line_color = lines->mutable_color();
    line_color->set_r(0.3);
    line_color->set_g(0.3);
    line_color->set_b(0.3);
    line_color->set_a(1.0);

    for (const auto& edge : graph_->edges) {
        const auto& from_node = graph_->nodes.at(edge.from_node_id);
        const auto& to_node = graph_->nodes.at(edge.to_node_id);

        auto* p1 = lines->add_points();
        p1->set_x(from_node.pos.x);
        p1->set_y(from_node.pos.y);
        p1->set_z(0.0);

        auto* p2 = lines->add_points();
        p2->set_x(to_node.pos.x);
        p2->set_y(to_node.pos.y);
        p2->set_z(0.0);
    }

    return *this;
}

RecordActor& RecordActor::WriteAgentStates(const models::AgentStates& agent_states) {
    auto& scene_update = ResetSceneUpdate();

    auto* deletion = scene_update.add_deletions();
    deletion->set_type(foxglove::SceneEntityDeletion::MATCHING_ID);
    deletion->set_id("agents");

    auto* agent_entity = scene_update.add_entities();
    agent_entity->set_frame_id("scene");
    agent_entity->set_id("agents");

    auto* timestamp = agent_entity->mutable_timestamp();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::duration<double>(context_->current_time));
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::duration<double>(context_->current_time)
        - std::chrono::duration<double>(seconds));
    timestamp->set_seconds(seconds.count());
    timestamp->set_nanos(nanos.count());

    for (const auto& [_, agent] : agent_states) {
        const auto& node = graph_->nodes.at(agent.node_id);

        {
            auto* cylinder = agent_entity->add_cylinders();

            auto* pose = cylinder->mutable_pose();
            auto* position = pose->mutable_position();
            position->set_x(node.pos.x);
            position->set_y(node.pos.y);
            position->set_z(0.05);

            auto* size = cylinder->mutable_size();
            size->set_x(0.3);
            size->set_y(0.3);
            size->set_z(0.05);

            auto* color = cylinder->mutable_color();
            float hue = std::fmod(agent.agent_id * 360.0f / agent_states.size(), 360.0f);
            auto rgb = hsv2rgb(hue, 0.8f, 0.95f);
            color->set_r(rgb.r);
            color->set_g(rgb.g);
            color->set_b(rgb.b);
            color->set_a(1.0);
        }

        {
            auto* text = agent_entity->add_texts();
            text->set_text("Agent " + std::to_string(agent.agent_id));
            text->set_billboard(true);
            text->set_font_size(16);
            text->set_scale_invariant(true);

            auto* color = text->mutable_color();
            color->set_r(255);
            color->set_g(255);
            color->set_b(255);
            color->set_a(1.0);

            auto* text_pose = text->mutable_pose();
            auto* text_pos = text_pose->mutable_position();
            text_pos->set_x(node.pos.x);
            text_pos->set_y(node.pos.y);
            text_pos->set_z(0.11);
        }
    }

    return *this;
}

foxglove::SceneUpdate& RecordActor::ResetSceneUpdate() {
    if (geom::Equal(scene_update_ts_, context_->current_time)) {
        return scene_update_;
    }

    WriteSceneUpdate();

    scene_update_.Clear();
    scene_update_ts_ = context_->current_time;

    return scene_update_;
}

RecordActor& RecordActor::WriteSceneUpdate() {
    auto msg_ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
                      std::chrono::duration<double>(scene_update_ts_))
                      .count();

    std::string serialized;
    scene_update_.SerializeToString(&serialized);

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

    return *this;
}

}  // namespace mapf::simulator