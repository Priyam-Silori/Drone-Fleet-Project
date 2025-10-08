#include "4_assignment_module.h"
#include "3_dijkstras_module.h" // ensure we can call Dijkstra::shortestDistance

#include <climits>
#include <iostream>
#include <cmath>
#include <limits>
using namespace std;

// ---------------- TaskComparator ----------------
bool TaskComparator::operator()(Task* a, Task* b) {
    return a->severity > b->severity; // smaller severity = higher priority
}

// ---------------- assignTasks ----------------
void assignTasks(
    unordered_map<string, Drone>& drones,
    const Graph& graph,
    vector<int>& hubs,
    priority_queue<Task*, vector<Task*>, TaskComparator>& taskQueue
) {
    const double alpha = 1.0; // weight for travel distance
    const double beta  = 0.5; // weight for excess capacity

    // 1️⃣ Recharge idle drones with battery = 0 (before assignment)
    for (auto& pair : drones) {
        Drone& d = pair.second;
        if (!d.available && d.battery == 0) {
            d.incrementIdle(); // increments idle_time & recharges
            if (d.battery > 0) d.available = true;
            cout << "Drone " << d.drone_id << " is charging. Battery: " << d.battery << "%\n";
        }
    }

    // 2️⃣ Assign tasks based on priority
    while (!taskQueue.empty()) {
        Task* t = taskQueue.top();
        taskQueue.pop();

        Drone* bestDrone = nullptr;
        double bestScore = numeric_limits<double>::infinity();

        for (auto& pair : drones) {
            Drone& d = pair.second;

            if (!d.available) continue;                // busy drones cannot take tasks
            if (t->payload_required > d.payload_capacity) continue;

            int travelDistance = Dijkstra::shortestDistance(graph, d.current_location, t->location);
            if (travelDistance < 0) continue;          // unreachable

            int consumed = d.batteryConsumption(travelDistance, t->payload_required);
            if (d.battery < consumed) continue;        // not enough battery

            int excessCapacity = (d.battery - consumed) + (d.payload_capacity - t->payload_required);
            double score = alpha * travelDistance - beta * excessCapacity;

            if (score < bestScore) {
                bestScore = score;
                bestDrone = &d;
            }
        }

        if (bestDrone != nullptr) {
            // Assign task
            int travelDistance = Dijkstra::shortestDistance(graph, bestDrone->current_location, t->location);
            int consumed = bestDrone->batteryConsumption(travelDistance, t->payload_required);

            bestDrone->updateBattery(consumed);
            bestDrone->updateLocation(t->location);
            bestDrone->available = false; // <-- now busy, fixed
            bestDrone->idle_time = 0;
            t->assigned = true;

            // Update nearest hub
            int nearestHub = hubs.empty() ? 0 : hubs[0];
            int minDist = INT_MAX;
            for (int h : hubs) {
                int dHub = Dijkstra::shortestDistance(graph, t->location, h);
                if (dHub >= 0 && dHub < minDist) {
                    minDist = dHub;
                    nearestHub = h;
                }
            }
            t->setNearestHub(nearestHub, (minDist == INT_MAX ? -1 : minDist));

            cout << "Task " << t->task_id << " assigned to Drone "
                 << bestDrone->drone_id
                 << ". Remaining battery: " << bestDrone->battery << "%\n";

        } else {
            cout << "No capable drone for task " << t->task_id
                 << ". Task remains unassigned.\n";
        }
    }

    // 3️⃣ Increment idle time & recharge only for drones still available
    for (auto& pair : drones) {
        Drone& d = pair.second;
        if (d.available) d.incrementIdle();
    }
}
