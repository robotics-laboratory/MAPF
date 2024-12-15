#pragma once

#include "graph/proto/graph.pb.h"

#include "geom/vector.h"

#include <boost/describe.hpp>
#include <boost/container_hash/hash.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

namespace mapf::graph {

using NodeId = uint32_t;
using NodeIdsList = std::vector<NodeId>;
using NodeIdsSet = std::unordered_set<NodeId>;

struct Node {
    Node();

    Node(NodeId id);

    Node(NodeId id, geom::Vec2 pos);

    Node(proto::Node node_proto);

    bool operator==(const Node& other) const;

    const NodeId id;
    const geom::Vec2 pos;
};

BOOST_DESCRIBE_STRUCT(Node, (), (id, pos));

using Nodes = std::unordered_map<NodeId, Node>;

using EdgeId = uint32_t;
using EdgeIds = std::vector<EdgeId>;

struct Edge {
    Edge();

    Edge(NodeId from_node_id, NodeId to_node_id);

    Edge(proto::Edge edge_proto);

    bool operator==(const Edge& other) const = default;

    const NodeId from_node_id;
    const NodeId to_node_id;
};

BOOST_DESCRIBE_STRUCT(Edge, (), (from_node_id, to_node_id));

using Edges = std::unordered_set<Edge, boost::hash<Edge>>;

class Graph {
  public:
    Graph();

    Graph(Nodes nodes, Edges edges);

    Graph(proto::Graph graph_proto);

    const NodeIdsSet& GetNeighbours(const NodeId& node_id) const;

    bool operator==(const Graph& other) const = default;

    Nodes nodes;
    Edges edges;

  private:
    std::unordered_map<NodeId, NodeIdsSet> neighbours_;
};

using GraphPtr = std::shared_ptr<Graph>;

struct Endpoints {
    Endpoints();

    Endpoints(NodeId from_node_id, NodeId to_node_id);

    Endpoints(proto::Endpoints endpoints_proto);

    bool operator==(const Endpoints& other) const = default;

    const NodeId from_node_id;
    const NodeId to_node_id;
};

}  // namespace mapf::graph