#include "4_assignment_module.h"
#include "3_dijkstras_module.h"

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
            cout << "Drone " << d.drone_id << " is charging. Battery: " << d.battery << "%\n";
        }
    }

    // 2️⃣ Assign tasks based on priority
    while (!taskQueue.empty()) {
        Task* t = taskQueue.top();
        taskQueue.pop();

        Drone* bestDrone = nullptr;
        double bestScore = numeric_limits<double>::infinity();

        // Find nearest hub for this task
        int nearestHub = hubs.empty() ? 0 : hubs[0];
        int minDistHub = INT_MAX;
        for (int h : hubs) {
            int dHub = Dijkstra::shortestDistance(graph, t->location, h);
            if (dHub >= 0 && dHub < minDistHub) {
                minDistHub = dHub;
                nearestHub = h;
            }
        }
        t->setNearestHub(nearestHub, (minDistHub == INT_MAX ? -1 : minDistHub));

        for (auto& pair : drones) {
            Drone& d = pair.second;

            if (!d.available) continue;                
            if (t->payload_required > d.payload_capacity) continue;

            int travelDistance = Dijkstra::shortestDistance(graph, d.current_location, t->location);
            if (travelDistance < 0) continue;          

            int batteryNeeded = d.batteryConsumption(travelDistance, t->payload_required);
            int totalBatteryNeeded = batteryNeeded + minDistHub; // battery to complete task + reach hub
            if (d.battery < totalBatteryNeeded) continue;     

            int excessCapacity = (d.battery - totalBatteryNeeded) + (d.payload_capacity - t->payload_required);
            double score = alpha * travelDistance - beta * excessCapacity;

            if (score < bestScore) {
                bestScore = score;
                bestDrone = &d;
            }
        }

        if (bestDrone != nullptr) {
            // Assign task
            int travelDistance = Dijkstra::shortestDistance(graph, bestDrone->current_location, t->location);
            int batteryUsed = bestDrone->batteryConsumption(travelDistance, t->payload_required);

            bestDrone->updateBattery(batteryUsed);
            bestDrone->updateLocation(t->location);
            bestDrone->available = false; 
            bestDrone->idle_time = 0;
            t->assigned = true;

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
