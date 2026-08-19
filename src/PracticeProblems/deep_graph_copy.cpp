#include <vector>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include "deep_graph_copy.h"
#include "Node.h"
using namespace std;

Node* cloneGraph(Node* node) {
    auto visited = unordered_set<int>{ };
    auto nodes_to_check = queue<Node*>{ };
    nodes_to_check.emplace(node);
    auto old_neighbors = unordered_map<int, vector<int>>{ };
    auto tracker = unordered_map<int, Node>{};
    while (nodes_to_check.size() != 0) {
        Node* current_node = nodes_to_check.front(); nodes_to_check.pop();
        auto new_node = new Node(current_node->val);
        vector<Node*> neighbors = current_node->neighbors;
        tracker.emplace(new_node->val, new_node);
        old_neighbors.emplace(new_node->val, neighbors);
        auto neigbors = current_node->neighbors;
        for (auto neighbor : neighbors) {
            if (!visited.contains(neighbor->val)) {
                nodes_to_check.push(neighbor);
                visited.insert(neighbor->val);
            }
        }
    }
    for (auto val_to_node : tracker) {
        auto current_node = tracker[val_to_node.first];
        for (auto neighbor_val : old_neighbors[current_node.val]) {
            auto neighbor = tracker[neighbor_val];
            current_node.neighbors.push_back(&neighbor);
        }
    }
    return new Node(tracker[node->val]);
}