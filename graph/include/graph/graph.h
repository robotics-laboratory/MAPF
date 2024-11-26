#pragma once

#include "graph/proto/graph.pb.h"

#include "geom/vector.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace mapf::graph {

using NodeId = uint32_t;
using NodeIds = std::vector<NodeId>;

struct Node {
    Node();

    Node(NodeId id, geom::Vec2 pos);

    Node(proto::Node node_proto);

    const NodeId id;
    const geom::Vec2 pos;
};

using Nodes = std::vector<Node>;

using EdgeId = uint32_t;
using EdgeIds = std::vector<EdgeId>;

struct Edge {
    Edge();

    Edge(NodeId from_node_id, NodeId to_node_id);

    Edge(proto::Edge edge_proto);

    const NodeId from_node_id;
    const NodeId to_node_id;
};

using Edges = std::vector<Edge>;

class Graph {
  public:
    Graph();

    Graph(Nodes nodes, Edges edges);

    Graph(proto::Graph graph_proto);

    const NodeIds& GetNeighbours(const NodeId& node_id) const;

    const Nodes nodes;
    const Edges edges;

  private:
    std::unordered_map<NodeId, NodeIds> neighbours_;
};

using GraphPtr = std::shared_ptr<Graph>;

struct Endpoints {
    Endpoints();

    Endpoints(NodeId from_node_id, NodeId to_node_id);

    Endpoints(proto::Endpoints endpoints_proto);

    const NodeId from_node_id;
    const NodeId to_node_id;
};

}  // namespace mapf::graph