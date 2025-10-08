#ifndef TASK_MODULE_H
#define TASK_MODULE_H

#include <string>
#include <iostream>
using namespace std;

class Task {
public:
    string task_id;          // Unique identifier
    int task_distance;       // Distance in kms (from drone start to task)
    int payload_required;    // Payload in kgs
    int severity;            // 1 = high, 2 = medium, 3 = low
    bool assigned;           // Is task assigned?
    int location;            // Node/location ID of task
    int nearestHub;          // Node/location ID of nearest charging hub
    int distanceToHub;       // Distance from task to nearest hub

    // Constructors
    Task();
    Task(string id, int distance, int payload, int sev, int loc = 0, int hub = 0, int distHub = 0);

    // Methods
    void markAssigned();
    void updateLocation(int new_location);
    void setNearestHub(int hub, int dist);

    // Print info
    friend ostream& operator<<(ostream& os, const Task& t);
};

#endif
