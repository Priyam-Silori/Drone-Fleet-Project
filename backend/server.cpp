// #include <cpprest/http_listener.h>
// #include <cpprest/json.h>
// #include <cpprest/asyncrt_utils.h> // for conversions
// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <queue>
// #include <string>
// #include "1_drone_module.h"
// #include "2_task_module.h"
// #include "3_dijkstras_module.h"
// #include "4_assignment_module.h"
// #include "5_file_module.h"

// using namespace web;
// using namespace web::http;
// using namespace web::http::experimental::listener;

// unordered_map<std::string, Drone> dronesMap;
// unordered_map<std::string, Task> tasksMap;
// std::vector<int> hubs = {5, 15, 25, 35, 45};

// // ---------------- Login ----------------
// void handle_login(http_request req) {
//     req.extract_json().then([](json::value j) {
//         std::string username = utility::conversions::to_utf8string(j[U("username")].as_string());
//         std::string password = utility::conversions::to_utf8string(j[U("password")].as_string());
//         json::value response;

//         if (username == "admin" && password == "admin123")
//             response[U("role")] = json::value::string(U("admin"));
//         else if (username == "user" && password == "user123")
//             response[U("role")] = json::value::string(U("user"));
//         else
//             response[U("role")] = json::value::string(U("invalid"));

//         return response;
//     }).then([req](json::value response) {
//         req.reply(status_codes::OK, response);
//     }).wait();
// }

// // ---------------- GET drones ----------------
// void handle_get_drones(http_request req) {
//     json::value arr = json::value::array();
//     int i = 0;
//     for (auto &entry : dronesMap) {
//         json::value d;
//         d[U("id")] = json::value::string(utility::conversions::to_string_t(entry.second.drone_id));
//         d[U("battery")] = json::value::number(entry.second.battery);
//         d[U("payload")] = json::value::number(entry.second.payload_capacity);
//         d[U("range")] = json::value::number(entry.second.range);
//         d[U("available")] = json::value::boolean(entry.second.available);
//         d[U("location")] = json::value::number(entry.second.current_location);
//         d[U("idle_time")] = json::value::number(entry.second.idle_time);
//         arr[i++] = d;
//     }
//     req.reply(status_codes::OK, arr);
// }

// // ---------------- GET tasks ----------------
// void handle_get_tasks(http_request req) {
//     json::value arr = json::value::array();
//     int i = 0;
//     for (auto &entry : tasksMap) {
//         json::value t;
//         t[U("id")] = json::value::string(utility::conversions::to_string_t(entry.second.task_id));
//         t[U("distance")] = json::value::number(entry.second.task_distance);
//         t[U("payload")] = json::value::number(entry.second.payload_required);
//         t[U("severity")] = json::value::number(entry.second.severity);
//         t[U("assigned")] = json::value::boolean(entry.second.assigned);
//         t[U("location")] = json::value::number(entry.second.location);
//         t[U("hub")] = json::value::number(entry.second.nearestHub);
//         t[U("distToHub")] = json::value::number(entry.second.distanceToHub);
//         arr[i++] = t;
//     }
//     req.reply(status_codes::OK, arr);
// }

// // ---------------- ADD drone ----------------
// void handle_add_drone(http_request req) {
//     req.extract_json().then([](json::value j) {
//         std::string id = utility::conversions::to_utf8string(j[U("id")].as_string());
//         int range = j[U("range")].as_integer();
//         int payload = j[U("payload")].as_integer();
//         int battery = j[U("battery")].as_integer();
//         bool avail = j[U("available")].as_bool();
//         int loc = j[U("location")].as_integer();
//         dronesMap[id] = Drone(id, range, payload, battery, avail, loc);
//         return json::value::string(U("Drone added successfully"));
//     }).then([req](json::value response) {
//         req.reply(status_codes::OK, response);
//     }).wait();
// }

// // ---------------- ADD task ----------------
// void handle_add_task(http_request req) {
//     req.extract_json().then([](json::value j) {
//         std::string id = utility::conversions::to_utf8string(j[U("id")].as_string());
//         int distance = j[U("distance")].as_integer();
//         int payload = j[U("payload")].as_integer();
//         int sev = j[U("severity")].as_integer();
//         int loc = j[U("location")].as_integer();

//         int nearestHub = hubs[0];
//         int minDist = abs(loc - hubs[0]);
//         for (int h : hubs) {
//             int d = abs(loc - h);
//             if (d < minDist) {
//                 minDist = d;
//                 nearestHub = h;
//             }
//         }

//         tasksMap[id] = Task(id, distance, payload, sev, loc, nearestHub, minDist);
//         return json::value::string(U("Task added successfully"));
//     }).then([req](json::value response) {
//         req.reply(status_codes::OK, response);
//     }).wait();
// }

// // ---------------- ASSIGN tasks ----------------
// void handle_assign_tasks(http_request req) {
//     priority_queue<Task*, std::vector<Task*>, TaskComparator> taskQueue;
//     for (auto &entry : tasksMap)
//         if (!entry.second.assigned)
//             taskQueue.push(&entry.second);

//     assignTasks(dronesMap, Graph{}, hubs, taskQueue);
//     req.reply(status_codes::OK, U("Tasks assigned successfully"));
// }

// // ---------------- STATUS ----------------
// void handle_status(http_request req) {
//     json::value resp;
//     resp[U("drones")] = json::value::array();
//     resp[U("tasks")] = json::value::array();
//     int i = 0;
//     for (auto &d : dronesMap) {
//         json::value drone;
//         drone[U("id")] = json::value::string(utility::conversions::to_string_t(d.second.drone_id));
//         drone[U("battery")] = json::value::number(d.second.battery);
//         drone[U("payload")] = json::value::number(d.second.payload_capacity);
//         drone[U("range")] = json::value::number(d.second.range);
//         drone[U("available")] = json::value::boolean(d.second.available);
//         drone[U("location")] = json::value::number(d.second.current_location);
//         drone[U("idle_time")] = json::value::number(d.second.idle_time);
//         resp[U("drones")][i++] = drone;
//     }
//     i = 0;
//     for (auto &t : tasksMap) {
//         json::value task;
//         task[U("id")] = json::value::string(utility::conversions::to_string_t(t.second.task_id));
//         task[U("distance")] = json::value::number(t.second.task_distance);
//         task[U("payload")] = json::value::number(t.second.payload_required);
//         task[U("severity")] = json::value::number(t.second.severity);
//         task[U("assigned")] = json::value::boolean(t.second.assigned);
//         task[U("location")] = json::value::number(t.second.location);
//         task[U("hub")] = json::value::number(t.second.nearestHub);
//         task[U("distToHub")] = json::value::number(t.second.distanceToHub);
//         resp[U("tasks")][i++] = task;
//     }
//     req.reply(status_codes::OK, resp);
// }

// // ---------------- MAIN ----------------
// // int main() {
// //     // Prepopulate drones/tasks
// //     dronesMap["D101"] = Drone("D101", 50, 50, 50, true, 0);
// //     dronesMap["D102"] = Drone("D102", 60, 60, 60, true, 0);
// //     dronesMap["D103"] = Drone("D103", 70, 70, 40, true, 0);
// //     dronesMap["D104"] = Drone("D104", 80, 80, 80, true, 0);
// //     dronesMap["D105"] = Drone("D105", 30, 30, 30, true, 0);
// //     dronesMap["D106"] = Drone("D106", 100, 90, 100, true, 0);

// //     tasksMap["T201"] = Task("T201", 25, 20, 1, 5, 5, 0);
// //     tasksMap["T202"] = Task("T202", 60, 50, 2, 10, 5, 0);
// //     tasksMap["T203"] = Task("T203", 80, 70, 3, 15, 15, 0);
// //     tasksMap["T204"] = Task("T204", 120, 60, 1, 20, 15, 0);
// //     tasksMap["T205"] = Task("T205", 30, 25, 2, 25, 25, 0);

// //     http_listener listener(U("http://localhost:8080"));

// //     listener.support(methods::GET, [](http_request req){
// //         std::string path = utility::conversions::to_utf8string(req.relative_uri().path());
// //         if(path == "/drones") handle_get_drones(req);
// //         else if(path == "/tasks") handle_get_tasks(req);
// //         else if(path == "/status") handle_status(req);
// //     });

// //     listener.support(methods::POST, [](http_request req){
// //         std::string path = utility::conversions::to_utf8string(req.relative_uri().path());
// //         if(path == "/addDrone") handle_add_drone(req);
// //         else if(path == "/addTask") handle_add_task(req);
// //         else if(path == "/assignTasks") handle_assign_tasks(req);
// //         else if(path == "/login") handle_login(req);
// //     });

// //     listener.open().wait();
// //     std::cout << "Server running at http://localhost:8080\n";

// //     // ---------------- Auto-open default browser ----------------
// // #ifdef _WIN32
// //     system("start http://localhost:8080");
// // #elif __APPLE__
// //     system("open http://localhost:8080");
// // #elif __linux__
// //     system("xdg-open http://localhost:8080");
// // #endif

// //     std::cout << "Press Enter to exit...\n";
// //     std::string line; std::getline(std::cin, line);

// //     return 0;
// // }

// #include <cpprest/http_listener.h>
// #include <iostream>

// using namespace web::http::experimental::listener;

// int main() {
//     try {
//         http_listener listener(U("http://127.0.0.1:8080"));
//         listener.support([](web::http::http_request req){
//             req.reply(web::http::status_codes::OK, U("Server is running!"));
//         });
//         listener.open().wait();
//         std::cout << "Server running at http://127.0.0.1:8080\nPress Enter to exit...\n";
//         std::string line; std::getline(std::cin, line);
//     } catch (std::exception &e) {
//         std::cout << "Exception: " << e.what() << std::endl;
//     }
// }



#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/asyncrt_utils.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <thread>
#include <cstdlib> // for system()
#include "1_drone_module.h"
#include "2_task_module.h"
#include "3_dijkstras_module.h"
#include "4_assignment_module.h"
#include "5_file_module.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

// ---------------- Global maps ----------------
unordered_map<string, Drone> dronesMap;
unordered_map<string, Task> tasksMap;
vector<int> hubs = {5, 15, 25, 35, 45};

// ---------------- Handlers ----------------
void handle_login(http_request req) {
    req.extract_json().then([](json::value j){
        string username = utility::conversions::to_utf8string(j[U("username")].as_string());
        string password = utility::conversions::to_utf8string(j[U("password")].as_string());
        json::value resp;
        if(username=="admin" && password=="admin123")
            resp[U("role")] = json::value::string(U("admin"));
        else if(username=="user" && password=="user123")
            resp[U("role")] = json::value::string(U("user"));
        else
            resp[U("role")] = json::value::string(U("invalid"));
        return resp;
    }).then([req](json::value resp){ req.reply(status_codes::OK, resp); }).wait();
}

void handle_get_drones(http_request req) {
    json::value arr = json::value::array();
    int i = 0;
    for(auto &entry : dronesMap){
        json::value d;
        d[U("id")] = json::value::string(utility::conversions::to_string_t(entry.second.drone_id));
        d[U("battery")] = json::value::number(entry.second.battery);
        d[U("payload")] = json::value::number(entry.second.payload_capacity);
        d[U("range")] = json::value::number(entry.second.range);
        d[U("available")] = json::value::boolean(entry.second.available);
        d[U("location")] = json::value::number(entry.second.current_location);
        d[U("idle_time")] = json::value::number(entry.second.idle_time);
        arr[i++] = d;
    }
    req.reply(status_codes::OK, arr);
}

void handle_get_tasks(http_request req){
    json::value arr = json::value::array();
    int i = 0;
    for(auto &entry : tasksMap){
        json::value t;
        t[U("id")] = json::value::string(utility::conversions::to_string_t(entry.second.task_id));
        t[U("distance")] = json::value::number(entry.second.task_distance);
        t[U("payload")] = json::value::number(entry.second.payload_required);
        t[U("severity")] = json::value::number(entry.second.severity);
        t[U("assigned")] = json::value::boolean(entry.second.assigned);
        t[U("location")] = json::value::number(entry.second.location);
        t[U("hub")] = json::value::number(entry.second.nearestHub);
        t[U("distToHub")] = json::value::number(entry.second.distanceToHub);
        arr[i++] = t;
    }
    req.reply(status_codes::OK, arr);
}

void handle_add_drone(http_request req){
    req.extract_json().then([](json::value j){
        string id = utility::conversions::to_utf8string(j[U("id")].as_string());
        int range = j[U("range")].as_integer();
        int payload = j[U("payload")].as_integer();
        int battery = j[U("battery")].as_integer();
        bool avail = j[U("available")].as_bool();
        int loc = j[U("location")].as_integer();
        dronesMap[id] = Drone(id, range, payload, battery, avail, loc);
        return json::value::string(U("Drone added successfully"));
    }).then([req](json::value resp){ req.reply(status_codes::OK, resp); }).wait();
}

void handle_add_task(http_request req){
    req.extract_json().then([](json::value j){
        string id = utility::conversions::to_utf8string(j[U("id")].as_string());
        int distance = j[U("distance")].as_integer();
        int payload = j[U("payload")].as_integer();
        int sev = j[U("severity")].as_integer();
        int loc = j[U("location")].as_integer();

        int nearestHub = hubs[0], minDist = abs(loc - hubs[0]);
        for(int h : hubs){
            int d = abs(loc - h);
            if(d < minDist){ minDist = d; nearestHub = h; }
        }

        tasksMap[id] = Task(id, distance, payload, sev, loc, nearestHub, minDist);
        return json::value::string(U("Task added successfully"));
    }).then([req](json::value resp){ req.reply(status_codes::OK, resp); }).wait();
}

void handle_assign_tasks(http_request req){
    priority_queue<Task*, vector<Task*>, TaskComparator> taskQueue;
    for(auto &entry : tasksMap) if(!entry.second.assigned) taskQueue.push(&entry.second);
    assignTasks(dronesMap, Graph{}, hubs, taskQueue);
    req.reply(status_codes::OK, U("Tasks assigned successfully"));
}

void handle_status(http_request req){
    json::value resp;
    resp[U("drones")] = json::value::array();
    resp[U("tasks")] = json::value::array();
    int i = 0;
    for(auto &d : dronesMap){
        json::value drone;
        drone[U("id")] = json::value::string(utility::conversions::to_string_t(d.second.drone_id));
        drone[U("battery")] = json::value::number(d.second.battery);
        drone[U("payload")] = json::value::number(d.second.payload_capacity);
        drone[U("range")] = json::value::number(d.second.range);
        drone[U("available")] = json::value::boolean(d.second.available);
        drone[U("location")] = json::value::number(d.second.current_location);
        drone[U("idle_time")] = json::value::number(d.second.idle_time);
        resp[U("drones")][i++] = drone;
    }
    i = 0;
    for(auto &t : tasksMap){
        json::value task;
        task[U("id")] = json::value::string(utility::conversions::to_string_t(t.second.task_id));
        task[U("distance")] = json::value::number(t.second.task_distance);
        task[U("payload")] = json::value::number(t.second.payload_required);
        task[U("severity")] = json::value::number(t.second.severity);
        task[U("assigned")] = json::value::boolean(t.second.assigned);
        task[U("location")] = json::value::number(t.second.location);
        task[U("hub")] = json::value::number(t.second.nearestHub);
        task[U("distToHub")] = json::value::number(t.second.distanceToHub);
        resp[U("tasks")][i++] = task;
    }
    req.reply(status_codes::OK, resp);
}

// ---------------- MAIN ----------------
int main() {
    // Prepopulate drones/tasks
    dronesMap["D101"] = Drone("D101", 50,50,50,true,0);
    dronesMap["D102"] = Drone("D102", 60,60,60,true,0);
    dronesMap["D103"] = Drone("D103", 70,70,40,true,0);

    tasksMap["T201"] = Task("T201",25,20,1,5,5,0);
    tasksMap["T202"] = Task("T202",60,50,2,10,5,0);

    http_listener listener(U("http://127.0.0.1:8080")); // reliable IPv4

    // Route GET
    listener.support(methods::GET, [](http_request req){
        string path = utility::conversions::to_utf8string(req.relative_uri().path());
        if(path=="/drones") handle_get_drones(req);
        else if(path=="/tasks") handle_get_tasks(req);
        else if(path=="/status") handle_status(req);
        else { req.reply(status_codes::NotFound,U("Unknown GET")); }
    });

    // Route POST
    listener.support(methods::POST, [](http_request req){
        string path = utility::conversions::to_utf8string(req.relative_uri().path());
        if(path=="/addDrone") handle_add_drone(req);
        else if(path=="/addTask") handle_add_task(req);
        else if(path=="/assignTasks") handle_assign_tasks(req);
        else if(path=="/login") handle_login(req);
        else { req.reply(status_codes::NotFound,U("Unknown POST")); }
    });

    try {
        listener.open().wait();
        cout << "Server running at http://127.0.0.1:8080\n";

        // Open default browser (reliable)
        system("explorer http://127.0.0.1:8080");

        // Keep server running
        while(true) this_thread::sleep_for(chrono::hours(24));
    } catch(exception &e){
        cerr << "Error starting server: " << e.what() << endl;
    }
}
