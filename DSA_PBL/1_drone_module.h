#ifndef DRONE_MODULE_H
#define DRONE_MODULE_H

#include <string>
#include <iostream>
using namespace std;

class Drone {
public:
    string drone_id;       // Unique identifier
    int range;             // Max distance in kms
    int payload_capacity;  // Max payload in kgs
    int battery;           // Current battery % (0-100)
    bool available;        // Is drone free for assignment?
    int current_location;  // Node/location ID
    int idle_time;         // Track how long drone is idle

    // Constructors
    Drone();
    Drone(string id, int r, int payload, int b, bool avail, int loc = 0);

    // Methods
    void resetDrone();  // Reset battery and availability
    int batteryConsumption(int distance, int task_payload); // Battery needed for task
    bool isAvailableForTask(int distance, int task_payload, int distanceToHub = 0); // Can drone handle task + reach hub?
    void updateBattery(int amount); // Reduce battery after task
    void updateLocation(int new_location); // Update current location
    void recharge(int amount);  // manually recharge battery
    void incrementIdle(int recharge_amount = 5); // increase idle time & recharge

    friend ostream& operator<<(ostream& os, const Drone& d);
};

#endif
