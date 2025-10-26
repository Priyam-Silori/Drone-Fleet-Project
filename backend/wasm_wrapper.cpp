// wasm_wrapper.cpp
#include "1_drone_module.h"
#include "2_task_module.h"
#include "4_assignment_module.h"
#include "5_file_module.h"
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// Global containers for drones and tasks
unordered_map<string, Drone> dronesMap;
vector<Task> tasksVector;

// ---------------- JSON helpers ----------------
string toJSON(const vector<Drone> &drones) {
    json jDrones = json::array();
    for (const auto &d : drones) {
        jDrones.push_back({
            {"drone_id", d.drone_id},
            {"range", d.range},
            {"payload_capacity", d.payload_capacity},
            {"battery", d.battery},
            {"available", d.available},
            {"current_location", d.current_location}
        });
    }
    return jDrones.dump();
}

string toJSON(const vector<Task> &tasks) {
    json jTasks = json::array();
    for (const auto &t : tasks) {
        jTasks.push_back({
            {"task_id", t.task_id},
            {"task_distance", t.task_distance},
            {"payload_required", t.payload_required},
            {"severity", t.severity},
            {"assigned", t.assigned},
            {"location", t.location},
            {"nearestHub", t.nearestHub},
            {"distanceToHub", t.distanceToHub}
        });
    }
    return jTasks.dump();
}

// ---------------- Wrapper functions exposed to JS ----------------
extern "C" {

// Add a drone
void wasmAddDrone(const char* id, int range, int payload, int battery, int location) {
    Drone d;
    d.drone_id = string(id);
    d.range = range;
    d.payload_capacity = payload;
    d.battery = battery;
    d.current_location = location;
    d.available = true;
    dronesMap[d.drone_id] = d;
}

// Add a task
void wasmAddTask(const char* id, int distance, int payload, int severity, int location) {
    Task t(string(id), distance, payload, severity, location, 0, 0);
    tasksVector.push_back(t);
}

// Assign tasks to drones
void wasmAssignTasks() {
    priority_queue<Task*, vector<Task*>, TaskComparator> pq;
    for (auto &t : tasksVector) {
        if (!t.assigned) pq.push(&t);
    }

    // TODO: provide a proper Graph object and hubs vector
    Graph g;           // initialize your actual graph
    vector<int> hubs;  // fill with your hub locations
    assignTasks(dronesMap, g, hubs, pq);
}

// Return drones as JSON string
const char* wasmGetDronesJSON() {
    static string jsonStr;
    vector<Drone> drones;
    for (auto &pair : dronesMap) drones.push_back(pair.second);
    jsonStr = toJSON(drones);
    return jsonStr.c_str();
}

// Return tasks as JSON string
const char* wasmGetTasksJSON() {
    static string jsonStr;
    jsonStr = toJSON(tasksVector);
    return jsonStr.c_str();
}

} // extern "C"
