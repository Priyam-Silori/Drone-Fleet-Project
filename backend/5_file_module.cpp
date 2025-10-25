#include "5_file_module.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

// ---------------- Drones ----------------
void saveDrones(const std::string &filename, const std::vector<Drone> &drones) {
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
    ofstream fout(filename);
    fout << setw(4) << jDrones;
    fout.close();
}

void loadDrones(const std::string &filename, std::vector<Drone> &drones) {
    ifstream fin(filename);
    if (!fin.is_open()) return;
    json jDrones;
    fin >> jDrones;
    drones.clear();
    for (const auto &j : jDrones) {
        Drone d;
        d.drone_id = j["drone_id"];
        d.range = j["range"];
        d.payload_capacity = j["payload_capacity"];
        d.battery = j["battery"];
        d.available = j["available"];
        d.current_location = j["current_location"];
        drones.push_back(d);
    }
}

// ---------------- Tasks ----------------
void saveTasks(const std::string &filename, const std::vector<Task> &tasks) {
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
    ofstream fout(filename);
    fout << setw(4) << jTasks;
    fout.close();
}

void loadTasks(const std::string &filename, std::vector<Task> &tasks) {
    ifstream fin(filename);
    if (!fin.is_open()) return;
    json jTasks;
    fin >> jTasks;
    tasks.clear();
    for (const auto &j : jTasks) {
        Task t;
        t.task_id = j["task_id"];
        t.task_distance = j["task_distance"];
        t.payload_required = j["payload_required"];
        t.severity = j["severity"];
        t.assigned = j["assigned"];
        t.location = j["location"];
        t.nearestHub = j["nearestHub"];
        t.distanceToHub = j["distanceToHub"];
        tasks.push_back(t);
    }
}
