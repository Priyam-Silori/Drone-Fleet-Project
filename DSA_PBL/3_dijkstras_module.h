#ifndef DIJKSTRA_MODULE_H
#define DIJKSTRA_MODULE_H

#include <vector>
#include <map>
#include <unordered_set>
#include <climits>
using namespace std;

// Graph represented as adjacency list: node -> vector of pairs (neighbor, weight)
typedef map<int, vector<pair<int, int>>> Graph;

class Dijkstra {
public:
    // Compute shortest distances from source to all nodes
    static map<int, int> shortestDistances(const Graph& g, int source);

    // Get shortest distance from source to target node
    static int shortestDistance(const Graph& g, int source, int target);

    // Get distance from a node to the nearest hub
    static int nearestHubDistance(const Graph& g, int node, const unordered_set<int>& hubs);
};

#endif
