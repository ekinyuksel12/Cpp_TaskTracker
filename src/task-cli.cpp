    #include <iostream>
    #include <fstream>
    #include <string>
    #include <vector>
    #include <sqlite3.h>

    #include "cxxopts.hpp"
    #include "nlohmann/json.hpp"
    #include "../include/task.hpp"

    #define JSON_DB_FILE "db.json"
    #define DB_FILE "tasks.db"

    using namespace std;
    using json = nlohmann::json;

    sqlite3* DB = nullptr;

    //Function that opens and connects to the database.
    int openDatabase(){
        int statusCode = sqlite3_open(DB_FILE, &DB);

        //Check errors happend during the opening of the database.
        if (statusCode != SQLITE_OK) {
            cerr << "Error openinng the database (" << DB_FILE << "): " << sqlite3_errmsg(DB) << endl;
            return statusCode;
        }
        return SQLITE_OK;
    }

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

    //Delete a task object from the database.
    void deleteTask (unsigned int id) {
        json j = readDb();

        //We search for an id match
        bool found = false;

        for (auto it = j.begin(); it != j.end(); ++it)
        {
            if ((*it).contains("id") && (*it)["id"] == id) {
                j.erase(it);
                found = true;
                break; // Break immediately after deleting to avoid iterator invalidation issues
            }
        }

        if (found) {
            writeDb(j);
            cout << "Task with ID " << id << " deleted successfully." << endl;
        } else {
            cout << "Error: Task with ID " << id << " not found." << endl;
        }
    }

    void createNewTask (string description) {
        Task newTask;
        newTask.setDescription(description);

        saveTask(newTask);
    }

    void updateTask(unsigned int id, string newDescription = "", taskStatus newStatus = taskStatus::NO_CHANGE) {
        json j = readDb();
        bool found = false;

        for (auto& element : j) {
            if (element.contains("id") && element["id"] == id) {
                // Update Description if provided (string is not empty)
                if (!newDescription.empty()) {
                    element["description"] = newDescription;
                }

                // Update Status if provided (status is not NO_CHANGE)
                if (newStatus != taskStatus::NO_CHANGE) {
                    element["status"] = newStatus;
                }

                // Update the 'updatedAt' timestamp
                time_t now;
                time(&now);
                element["updatedAt"] = now;

                found = true;
                break;
            }
        }

        if (found) {
            writeDb(j);
            cout << "Task " << id << " updated successfully." << endl;
        } else {
            cout << "Error: Task with ID " << id << " not found or no changes provided." << endl;
        }
    }

    int main (int argc, char* argv[]){
        initializeDB();

        //Create a cxxopt Options object and add options to it.
        cxxopts::Options options ("TaskTracker", "A command line todo/task tracker written in C++ ");

        options.add_options()
            ("a,add", "Create a new task with given description", cxxopts::value<std::string>())
            ("d,delete", "Delete a task at spesific id", cxxopts::value<unsigned int>())
            ("u,update", "Update a task at spesific id", cxxopts::value<unsigned int>())
            ("c,complete", "Complete a task at spesific id", cxxopts::value<unsigned int>())
            ("name", "New name for update (Optional if provided as argument)", cxxopts::value<std::string>())
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
        
        //Handling the --delete flag.
        if (result.count("delete")) {
            unsigned int id = result["delete"].as<unsigned int>();
            deleteTask(id);
        }
        
        //Handling the --update flag.
        if (result.count("update")) {
            unsigned int id = result["update"].as<unsigned int>();
            if (result.count("name")) {
                string newDesc = result["name"].as<std::string>();
                updateTask(id, newDesc);
            } else {
                cout << "Error: Please provide a new description using --name" << endl;
            }
        }
 
        //Handling the --complete flag.
        if (result.count("complete")) {
            unsigned int id = result["complete"].as<unsigned int>();
            updateTask(id, "", taskStatus::DONE);
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