#include "5_file_module.h"
#include <fstream>
#include <iostream>

using namespace std;

// ---------------- Drones ----------------
void loadDrones(const std::string &filename, std::vector<Drone> &drones) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) return;

    drones.clear();
    while (!fin.eof()) {
        Drone d;
        fin >> d.drone_id >> d.range >> d.payload_capacity >> d.battery >> d.available >> d.current_location;
        if (!d.drone_id.empty())
            drones.push_back(d);
    }
    fin.close();
}

void saveDrones(const std::string &filename, const std::vector<Drone> &drones) {
    ofstream fout(filename.c_str());
    for (size_t i = 0; i < drones.size(); i++) {
        const Drone &d = drones[i];
        fout << d.drone_id << " "
             << d.range << " "
             << d.payload_capacity << " "
             << d.battery << " "
             << d.available << " "
             << d.current_location << endl;
    }
    fout.close();
}

// ---------------- Tasks ----------------
void loadTasks(const std::string &filename, std::vector<Task> &tasks) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) return;

    tasks.clear();
    while (!fin.eof()) {
        Task t;
        fin >> t.task_id >> t.task_distance >> t.payload_required >> t.severity 
            >> t.assigned >> t.location >> t.nearestHub >> t.distanceToHub;
        if (!t.task_id.empty())
            tasks.push_back(t);
    }
    fin.close();
}

void saveTasks(const std::string &filename, const std::vector<Task> &tasks) {
    ofstream fout(filename.c_str());
    for (size_t i = 0; i < tasks.size(); i++) {
        const Task &t = tasks[i];
        fout << t.task_id << " "
             << t.task_distance << " "
             << t.payload_required << " "
             << t.severity << " "
             << t.assigned << " "
             << t.location << " "
             << t.nearestHub << " "
             << t.distanceToHub << endl;
    }
    fout.close();
}
