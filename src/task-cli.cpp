#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cxxopts.hpp"
#include "nlohmann/json.hpp"
#include "../include/task.hpp"

#define JSON_DB_FILE "db.json"

using namespace std;
using json = nlohmann::json;

void initializeDB() {
    if (!std::filesystem::exists(JSON_DB_FILE)) {
        ofstream oFile(JSON_DB_FILE);
        oFile << "[]";
        oFile.close();
    }
}

//Get the next task ID by reading the last task's ID from the json file.
unsigned int getNextTaskID(const json& j) {
    if (j.empty()) {
        return 1;
    }
    
    else {
        return j.back().at("id").get<unsigned int>() + 1;
    }
}

// Helper function to read the JSON file safely
json readDb() {
    ifstream iFile(JSON_DB_FILE);
    if (!iFile.is_open()) {
        return json::array();
    }
    
    // Check if file is empty
    if (iFile.peek() == ifstream::traits_type::eof()) {
        return json::array();
    }

    try {
        return json::parse(iFile);
    } catch (json::parse_error& e) {
        return json::array();
    }
}

// Helper function to write to the JSON file
void writeDb(const json& j) {
    ofstream oFile(JSON_DB_FILE);
    oFile << j.dump(4);
    oFile.close();
}

//Save task object to end of the json file and modify its id.
void saveTask (Task task) {
    json j = readDb();

    task.id = getNextTaskID(j);
    j.push_back(task);

    writeDb(j);
}

void createNewTask (string description) {
    Task newTask;
    newTask.setDescription(description);

    saveTask(newTask);
}

int main (int argc, char* argv[]){
    initializeDB();

    //Create a cxxopt Options object and add options to it.
    cxxopts::Options options ("TaskTracker", "A command line todo/task tracker written in C++ ");

    options.add_options()
        ("a,add", "Create a new task with given description", cxxopts::value<std::string>())
        ("l,list", "List all tasks")
        ("h,help", "Print help screen");
    
    auto result = options.parse(argc, argv);

    //Handling the --help flag.
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    //Handling the --add flag.
    if (result.count("add")) {
        string description = result["add"].as<std::string>();
        createNewTask(description);
    }
    
    //Handling the --list flag.
    if (result.count("list")) {
        ifstream jsonFile(JSON_DB_FILE);
        json j = json::parse(jsonFile);

        std::vector<Task> tasks = j.get<std::vector<Task>>();

        for (auto& task: tasks)
        {
            cout << task << endl;
        }
    }
}