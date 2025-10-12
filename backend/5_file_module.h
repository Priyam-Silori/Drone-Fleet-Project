#ifndef FILE_MODULE_H
#define FILE_MODULE_H

#include <vector>
#include <string>
#include "1_drone_module.h"
#include "2_task_module.h"

// Drones
void loadDrones(const std::string &filename, std::vector<Drone> &drones);
void saveDrones(const std::string &filename, const std::vector<Drone> &drones);

// Tasks
void loadTasks(const std::string &filename, std::vector<Task> &tasks);
void saveTasks(const std::string &filename, const std::vector<Task> &tasks);

#endif
