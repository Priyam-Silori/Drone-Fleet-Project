#include "3_dijkstras_module.h"
#include <set>

map<int, int> Dijkstra::shortestDistances(const Graph& g, int source) {
    map<int, int> dist;

    for (auto it = g.begin(); it != g.end(); ++it) {
        dist[it->first] = INT_MAX;
    }
    dist[source] = 0;

    set<pair<int, int>> s; // {distance, node}
    s.insert({0, source});

    while (!s.empty()) {
        auto it = s.begin();
        int u = it->second;
        s.erase(it);

        auto nodeIt = g.find(u);
        if (nodeIt != g.end()) {
            for (auto &neighbor : nodeIt->second) {
                int v = neighbor.first;
                int w = neighbor.second;

                if (dist[u] + w < dist[v]) {
                    if (dist[v] != INT_MAX)
                        s.erase({dist[v], v});
                    dist[v] = dist[u] + w;
                    s.insert({dist[v], v});
                }
            }
        }
    }

    return dist;
}

int Dijkstra::shortestDistance(const Graph& g, int source, int target) {
    map<int, int> dist = shortestDistances(g, source);
    if (dist.find(target) != dist.end())
        return dist[target];
    else
        return -1; // unreachable
}

// 🔹 New helper: compute distance to nearest hub
int Dijkstra::nearestHubDistance(const Graph& g, int node, const unordered_set<int>& hubs) {
    int minDist = INT_MAX;
    for (int hub : hubs) {
        int d = shortestDistance(g, node, hub);
        if (d != -1 && d < minDist) minDist = d;
    }
    return minDist;
}
