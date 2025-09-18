#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <task.hpp>

using namespace std;
using json = nlohmann::json;

//Save task object to end of the json file and modify its id.
//TODO: Create a separete function to get the last id.
void saveTask (Task task) {
    ifstream iFile("example.json");
    json j = json::parse(iFile);
    iFile.close();

    ofstream oFile("example.json");
    cout << j << endl;

    task.id = j.size();
    j.push_back(task);

    oFile << j.dump(4);
    oFile.close();
}

void createNewTask (string description) {
    Task newTask;
    newTask.setDescription(description);

    saveTask(newTask);
}

int main (int argc, char* argv[]){
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
        ifstream jsonFile("example.json");
        json j = json::parse(jsonFile);

        std::vector<Task> tasks = j.get<std::vector<Task>>();

        for (auto& task: tasks)
        {
            cout << task << endl;
        }
    }
}