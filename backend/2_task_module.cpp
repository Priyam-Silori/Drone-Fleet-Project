#include "2_task_module.h"

// Default constructor
Task::Task() {
    task_id = "";
    task_distance = 0;
    payload_required = 0;
    severity = 3; // default low severity
    assigned = false;
    location = 0;
    nearestHub = 0;
    distanceToHub = 0;
}

// Parameterized constructor
Task::Task(string id, int distance, int payload, int sev, int loc, int hub, int distHub) {
    task_id = id;
    task_distance = distance;
    payload_required = payload;
    severity = sev;
    assigned = false;
    location = loc;
    nearestHub = hub;
    distanceToHub = distHub;
}

// Mark task as assigned
void Task::markAssigned() {
    assigned = true;
}

// Update task location
void Task::updateLocation(int new_location) {
    location = new_location;
}

// Set nearest hub and distance
void Task::setNearestHub(int hub, int dist) {
    nearestHub = hub;
    distanceToHub = dist;
}

// Print task info
ostream& operator<<(ostream& os, const Task& t) {
    os << "Task ID: " << t.task_id
       << ", Distance: " << t.task_distance
       << ", Payload: " << t.payload_required
       << ", Severity: " << t.severity
       << ", Assigned: " << (t.assigned ? "Yes" : "No")
       << ", Location: " << t.location
       << ", Nearest Hub: " << t.nearestHub
       << ", Distance to Hub: " << t.distanceToHub;
    return os;
}
