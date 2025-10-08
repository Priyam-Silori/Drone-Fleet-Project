// ---------------- Drone Assignment Module ----------------
// ASSUMPTION: 1 km distance consumes battery proportional to payload
// Distance between drone and task will come from Dijkstra module later

#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

// ---------------- Drone Class ----------------
class Drone {
public:
    string drone_id;  
    int range;              // max distance it can travel in kms
    int payload_capacity;   // in kgs
    int battery;            // current battery in kms
    bool available;         // can this drone be assigned?
    int current_location;   // placeholder for future Dijkstra integration

    Drone() {
        battery = 100;
        available = true;
        current_location = 0;
    }

    Drone(string id, int r, int payload, int b, bool avail, int loc = 0) {
        drone_id = id;
        range = r;
        payload_capacity = payload;
        battery = b;
        available = avail;
        current_location = loc;
    }
};

// ---------------- Task Class ----------------
class Task {
public:
    string task_id;
    int task_distance;          // distance in kms
    int payload_required;       // in kgs
    bool assigned;              // has this task been assigned?
    int severity;               // 1 = high, 2 = medium, 3 = low
    int task_location;          // placeholder for future Dijkstra integration

    Task() {
        task_id = "";
        task_distance = 0;
        payload_required = 0;
        assigned = false;
        severity = 3;
        task_location = 0;
    }

    Task(string id, int distance, int payload, bool assign, int severity_level, int loc = 0) {
        task_id = id;
        task_distance = distance;
        payload_required = payload;
        assigned = assign;
        severity = severity_level;
        task_location = loc;
    }
};

// ---------------- Priority Comparator ----------------
struct TaskComparator {
    bool operator()(Task* a, Task* b) {
        // smaller severity number = higher priority
        return a->severity > b->severity;
    }
};

// ---------------- Assignment Function ----------------
void assignTasks(unordered_map<int, Drone> &drones,
                 priority_queue<Task*, vector<Task*>, TaskComparator> &taskQueue) {

    while (!taskQueue.empty()) {
        Task* t = taskQueue.top();
        taskQueue.pop();

        Drone* bestDrone = nullptr;
        int minExcessCapacity = INT_MAX;

        for (auto &dEntry : drones) {
            Drone &d = dEntry.second;

            // Battery consumption formula
            int consumed = t->task_distance * (1 + (double)t->payload_required / d.payload_capacity);

            // Check if drone can handle the task
            if (d.available && d.range >= t->task_distance &&
                d.payload_capacity >= t->payload_required && d.battery >= consumed) {

                // Find drone that fits task most efficiently
                int excessCapacity = (d.range - t->task_distance) + (d.payload_capacity - t->payload_required);
                if (excessCapacity < minExcessCapacity) {
                    minExcessCapacity = excessCapacity;
                    bestDrone = &d;
                }
            }
        }

        if (bestDrone) {
            bestDrone->available = false;
            bestDrone->battery -= t->task_distance * (1 + (double)t->payload_required / bestDrone->payload_capacity);
            t->assigned = true;

            cout << "Assigned Task " << t->task_id << " (Severity " << t->severity 
                 << ") to Drone " << bestDrone->drone_id << endl;
        } else {
            cout << "Task " << t->task_id << " (Severity " << t->severity 
                 << ") could not be assigned to any drone.\n";
        }
    }
}

// ---------------- Main ----------------
int main() {
    unordered_map<int, Drone> drones;
    unordered_map<int, Task> tasks;

    // --- Add drones ---
    drones[101] = Drone("D101", 100, 100, 100, true, 0);
    drones[102] = Drone("D102", 80, 80, 80, true, 0);
    drones[103] = Drone("D103", 50, 50, 50, true, 0);

    // --- Add tasks ---
    // Placeholder distances until Dijkstra module is integrated
    int distance1 = 50;
    int distance2 = 60;
    int distance3 = 80;

    tasks[201] = Task("T201", distance1, 40, false, 2, 10);
    tasks[202] = Task("T202", distance2, 50, false, 1, 20);
    tasks[203] = Task("T203", distance3, 70, false, 3, 30);

    // --- Create priority queue of tasks ---
    priority_queue<Task*, vector<Task*>, TaskComparator> taskQueue;
    for (auto &entry : tasks) {
        taskQueue.push(&entry.second);
    }

    // --- Assign tasks to drones ---
    assignTasks(drones, taskQueue);

    // --- Print final drone status ---
    cout << "\n--- Drone Status ---\n";
    for (auto &entry : drones) {
        Drone &d = entry.second;
        cout << d.drone_id << " | Battery: " << d.battery 
             << " | Available: " << (d.available ? "Yes" : "No") << endl;
    }

    // --- Print final task status ---
    cout << "\n--- Task Status ---\n";
    for (auto &entry : tasks) {
        Task &t = entry.second;
        cout << t.task_id << " | Assigned: " << (t.assigned ? "Yes" : "No") << endl;
    }

    return 0;
}
