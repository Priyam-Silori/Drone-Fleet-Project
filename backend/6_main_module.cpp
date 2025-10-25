#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <climits>
#include <iomanip>
#include "1_drone_module.h"
#include "2_task_module.h"
#include "3_dijkstras_module.h"
#include "4_assignment_module.h"
#include "5_file_module.h" 

using namespace std;

// Helper to print drones
void printDrones(unordered_map<string, Drone>& drones) {
    cout << "\n================== DRONE STATUS ==================\n";
    cout << left
         << setw(8) << "ID"
         << setw(10) << "Battery"
         << setw(10) << "Payload"
         << setw(8) << "Range"
         << setw(10) << "Available"
         << setw(10) << "Location"
         << setw(10) << "IdleTime"
         << "\n-------------------------------------------------\n";

    for (auto &entry : drones) {
        Drone &d = entry.second;
        cout << left
             << setw(8) << d.drone_id
             << setw(10) << d.battery
             << setw(10) << d.payload_capacity
             << setw(8) << d.range
             << setw(10) << (d.available ? "Yes" : "No")
             << setw(10) << d.current_location
             << setw(10) << d.idle_time
             << "\n";
    }
    cout << "=================================================\n";
}

// Helper to print tasks
void printTasks(unordered_map<string, Task>& tasks) {
    cout << "\n================== TASK STATUS ==================\n";
    cout << left
         << setw(8) << "ID"
         << setw(10) << "Distance"
         << setw(10) << "Payload"
         << setw(8) << "Severity"
         << setw(10) << "Assigned"
         << setw(10) << "Location"
         << setw(10) << "Hub"
         << setw(10) << "DistToHub"
         << "\n-------------------------------------------------\n";

    for (auto &entry : tasks) {
        Task &t = entry.second;
        cout << left
             << setw(8) << t.task_id
             << setw(10) << t.task_distance
             << setw(10) << t.payload_required
             << setw(8) << t.severity
             << setw(10) << (t.assigned ? "Yes" : "No")
             << setw(10) << t.location
             << setw(10) << t.nearestHub
             << setw(10) << t.distanceToHub
             << "\n";
    }
    cout << "=================================================\n";
}

int main() {
    unordered_map<string, Drone> dronesMap;
    unordered_map<string, Task> tasksMap;
    vector<int> hubs = {5, 15, 25, 35, 45}; // hub locations

    // ---------------- Predefined drones ----------------
    dronesMap["D101"] = Drone("D101", 50, 50, 50, true, 0);
    dronesMap["D102"] = Drone("D102", 60, 60, 60, true, 0);
    dronesMap["D103"] = Drone("D103", 70, 70, 40, true, 0); // low battery
    dronesMap["D104"] = Drone("D104", 80, 80, 80, true, 0);
    dronesMap["D105"] = Drone("D105", 30, 30, 30, true, 0); // limited range
    dronesMap["D106"] = Drone("D106", 100, 90, 100, true, 0);

    // ---------------- Predefined tasks ----------------
    tasksMap["T201"] = Task("T201", 25, 20, 1, 5, 5, 0);    // assignable
    tasksMap["T202"] = Task("T202", 60, 50, 2, 10, 5, 0);   // assignable to high-range drone
    tasksMap["T203"] = Task("T203", 80, 70, 3, 15, 15, 0);  // may not assign (payload)
    tasksMap["T204"] = Task("T204", 120, 60, 1, 20, 15, 0); // unassignable (too far)
    tasksMap["T205"] = Task("T205", 30, 25, 2, 25, 25, 0);  // assignable


        // ---------------- Simple Graph ----------------
    Graph graph;
    graph[0] = {{5, 25}, {10, 60}, {15, 80}, {20, 120}, {25, 30}};
    graph[5] = {{0, 25}, {10, 35}};
    graph[10] = {{0, 60}, {5, 35}, {15, 20}};
    graph[15] = {{0, 80}, {10, 20}, {20, 40}};
    graph[20] = {{0, 120}, {15, 40}, {25, 50}};
    graph[25] = {{0, 30}, {20, 50}};

    // ---------------- Show initial status ----------------
    printDrones(dronesMap);
    printTasks(tasksMap);

    // ---------------- Interactive addition ----------------
    char choice;
    do {
        cout << "\nDo you want to add a new drone? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            string id; int range, payload, battery, loc; bool avail;
            cout << "Drone ID: "; cin >> id;
            cout << "Range: "; cin >> range;
            cout << "Payload: "; cin >> payload;
            cout << "Battery: "; cin >> battery;
            cout << "Available (1=Yes,0=No): "; cin >> avail;
            cout << "Current Location: "; cin >> loc;
            dronesMap[id] = Drone(id, range, payload, battery, avail, loc);
            cout << "Drone " << id << " added successfully!\n";
        }
    } while (choice == 'y' || choice == 'Y');

    do {
        cout << "\nDo you want to add a new task? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            string id; int distance, payload, sev, loc;
            cout << "Task ID: "; cin >> id;
            cout << "Distance: "; cin >> distance;
            cout << "Payload Required: "; cin >> payload;
            cout << "Severity (1=High,2=Med,3=Low): "; cin >> sev;
            cout << "Location: "; cin >> loc;

            // Nearest hub
            int nearestHub = hubs[0];
            int minDist = abs(loc - hubs[0]);
            for (int h : hubs) {
                int d = abs(loc - h);
                if (d < minDist) {
                    minDist = d;
                    nearestHub = h;
                }
            }
            tasksMap[id] = Task(id, distance, payload, sev, loc, nearestHub, minDist);
            cout << "Task " << id << " added successfully!\n";
        }
    } while (choice == 'y' || choice == 'Y');



    // ---------------- Task Queue ----------------
    priority_queue<Task*, vector<Task*>, TaskComparator> taskQueue;
    for (auto &entry : tasksMap)
        if (!entry.second.assigned)
            taskQueue.push(&entry.second);

    // ---------------- Assign tasks ----------------
    assignTasks(dronesMap, graph, hubs, taskQueue);

    // ---------------- Final status ----------------
    printDrones(dronesMap);
    printTasks(tasksMap);

     // ✅ Convert maps → vectors for JSON saving
    vector<Drone> droneVec;
    for (auto &d : dronesMap) droneVec.push_back(d.second);

    vector<Task> taskVec;
    for (auto &t : tasksMap) taskVec.push_back(t.second);

    // ✅ Save JSON files
    saveDrones("drones.json", droneVec);
    saveTasks("tasks.json", taskVec);

    cout << "\n✅ JSON files exported successfully (drones.json, tasks.json)\n";

    return 0;
}
