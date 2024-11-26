#include "graph/graph.h"

#include <iterator>
#include <utility>

namespace mapf::graph {

Node::Node() : id{0}, pos{geom::Vec2()} {}

Node::Node(NodeId id, geom::Vec2 pos) : id{std::move(id)}, pos{std::move(pos)} {}

Node::Node(proto::Node node_proto) :
    id{std::move(node_proto.id())}, pos{std::move(node_proto.pos())} {}

Edge::Edge() : from_node_id{0}, to_node_id{0} {}

Edge::Edge(NodeId from_node_id, NodeId to_node_id) :
    from_node_id{std::move(from_node_id)}, to_node_id{std::move(to_node_id)} {}

Edge::Edge(proto::Edge edge_proto) :
    from_node_id{std::move(edge_proto.from_node_id())},
    to_node_id{std::move(edge_proto.to_node_id())} {}

Graph::Graph() : nodes(), edges(), neighbours_() {}

Graph::Graph(Nodes nodes, Edges edges) : nodes(std::move(nodes)), edges(std::move(edges)) {
    for (const auto& [from_node_id, to_node_id] : edges) {
        neighbours_[from_node_id].emplace_back(to_node_id);
    }
}

Graph::Graph(proto::Graph graph_proto) :
    Graph(
        Nodes(
            std::make_move_iterator(graph_proto.nodes().begin()),
            std::make_move_iterator(graph_proto.nodes().end())),
        Edges(
            std::make_move_iterator(graph_proto.edges().begin()),
            std::make_move_iterator(graph_proto.edges().end()))) {}

const NodeIds& Graph::GetNeighbours(const NodeId& node_id) const { return neighbours_.at(node_id); }

Endpoints::Endpoints() : from_node_id{0}, to_node_id{0} {}

Endpoints::Endpoints(NodeId from_node_id, NodeId to_node_id) :
    from_node_id{from_node_id}, to_node_id{to_node_id} {}

Endpoints::Endpoints(proto::Endpoints endpoints_proto) :
    from_node_id{std::move(endpoints_proto.from_node_id())},
    to_node_id{std::move(endpoints_proto.to_node_id())} {}

}  // namespace mapf::graph