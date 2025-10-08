#ifndef ASSIGNMENT_MODULE_H
#define ASSIGNMENT_MODULE_H

#include <unordered_map>
#include <queue>
#include <vector>
#include "1_drone_module.h"
#include "2_task_module.h"
#include "3_dijkstras_module.h" // Graph typedef and Dijkstra class

// Comparator for task priority (smaller severity = higher priority)
struct TaskComparator {
    bool operator()(Task* a, Task* b);
};

// Assign tasks to drones considering distance (via Dijkstra), capacity, and hubs
// NOTE: we now accept a Graph reference so we can compute real shortest paths.
void assignTasks(
    std::unordered_map<std::string, Drone>& drones,
    const Graph& graph,
    std::vector<int>& hubs, // list of hub locations
    std::priority_queue<Task*, std::vector<Task*>, TaskComparator>& taskQueue
);

#endif
