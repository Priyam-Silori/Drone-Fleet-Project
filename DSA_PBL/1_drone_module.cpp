

#include "1_drone_module.h"

// Default constructor
Drone::Drone() {
    drone_id = "";
    range = 0;
    payload_capacity = 0;
    battery = 100;
    available = true;
    current_location = 0;
    idle_time= 0 ;
}

// Parameterized constructor
Drone::Drone(string id, int r, int payload, int b, bool avail, int loc) {
    drone_id = id;
    range = r;
    payload_capacity = payload;
    battery = b;
    available = avail;
    current_location = loc;
    idle_time = 0 ;  
}

// Reset drone to available state
void Drone::resetDrone() {
    available = true;
    battery = 100; // fully charged
    idle_time= 0 ; 
}

// Battery needed for a task
int Drone::batteryConsumption(int distance, int task_payload) {
    return static_cast<int>(distance * (1.0 + ((double)task_payload / payload_capacity)));
}

// Check if drone can handle task + reach nearest hub
bool Drone::isAvailableForTask(int distance, int task_payload, int distanceToHub) {
    if (!available) return false;
    if (task_payload > payload_capacity) return false;

    int requiredBattery = batteryConsumption(distance, task_payload) + distanceToHub;
    if (battery < requiredBattery) return false; // not enough battery to finish task & reach hub

    return true;
}

// Reduce battery after task
void Drone::updateBattery(int amount) {
    battery -= amount;
    if (battery < 0) battery = 0;
}

// Update current location
void Drone::updateLocation(int new_location) {
    current_location = new_location;
}

// Recharge battery manually
void Drone::recharge(int amount) {
    battery += amount;
    if (battery > 100) battery = 100;
}

// Increment idle time and recharge automatically
void Drone::incrementIdle(int recharge_amount){
    idle_time++; 
    recharge(recharge_amount); 
    available = true; 
}

// Print drone info
ostream& operator<<(ostream& os, const Drone& d) {
    os << "Drone ID: " << d.drone_id
       << ", Battery: " << d.battery
       << ", Payload: " << d.payload_capacity
       << ", Range: " << d.range
       << ", Available: " << (d.available ? "Yes" : "No")
       << ", Location: " << d.current_location
       << ", Idle time: " << d.idle_time;
    return os;
}
