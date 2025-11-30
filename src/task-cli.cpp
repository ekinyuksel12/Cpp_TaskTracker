    #include <iostream>
    #include <fstream>
    #include <string>
    #include <vector>
    #include <sqlite3.h>

    #include "cxxopts.hpp"
    #include "nlohmann/json.hpp"
    #include "../include/task.hpp"

    #define TO_DO 1
    #define NO_CHAGE 2
    #define DONE 3

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
        if (openDatabase() != SQLITE_OK) {
            return; //Fail to open DB.
        }

        char* errMsg = 0;

        //1. Create the status table.
        const char* sql_status =
            "CREATE TABLE IF NOT EXISTS status ("
            "ID INTEGER PRIMARY KEY,"
            "NAME TEXT NOT NULL UNIQUE"
            ");";
        
        sqlite3_exec(DB, sql_status, 0, 0, &errMsg);

        // 2. Insert default statuses (id=1 is to-do, 2 is no_change, 3 is done)
        const char* sql_insert_status = 
            "INSERT OR IGNORE INTO status (ID, NAME) VALUES (1, 'to-do');"
            "INSERT OR IGNORE INTO status (ID, NAME) VALUES (2, 'no_change');"
            "INSERT OR IGNORE INTO status (ID, NAME) VALUES (3, 'done');";

        sqlite3_exec(DB, sql_insert_status, 0, 0, &errMsg);

        // 3. Create the main tasks table
        const char* sql_tasks = 
            "CREATE TABLE IF NOT EXISTS tasks ("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "DESCRIPTION TEXT NOT NULL,"
            "STATUS_ID INTEGER NOT NULL DEFAULT 1," // Default to 'Pending' (ID 1)
            "CREATED_AT INTEGER NOT NULL,"
            "UPDATED_AT INTEGER NOT NULL,"
            "FOREIGN KEY (status_id) REFERENCES status(id)"
            ");";

        if (sqlite3_exec(DB, sql_tasks, 0, 0, &errMsg) != SQLITE_OK) {
                std::cerr << "SQL Error: " << errMsg << std::endl;
                sqlite3_free(errMsg);
        }
    }

    //Save task object to end of the json file and modify its id.
    void saveTask (Task task) {
        if (openDatabase() != SQLITE_OK) return;

        //Creating the statement skeleton.
        const char* sql = "INSERT INTO tasks (DESCRIPTION, STATUS_ID, CREATED_AT, UPDATED_AT) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        // 1. Prepare the SQL statement.
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL Prepare Error: " << sqlite3_errmsg(DB) << std::endl;
            return;
        }

        // 2. Bind the values to the placeholders.
        sqlite3_bind_text(stmt, 1, task.description.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, TO_DO);
        sqlite3_bind_int64(stmt, 3, task.createdAt);
        sqlite3_bind_int64(stmt, 4, task.updatedAt);

        // 3. Execute the statement.
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "SQL Execute Error: " << sqlite3_errmsg(DB) << std::endl;
        }

        sqlite3_finalize(stmt);
    }

    //Delete a task object from the database.
    void deleteTask (unsigned int id) {
        if (openDatabase() != SQLITE_OK) return;

        //Creating the statement skeleton.
        const char* sql = "DELETE FROM tasks WHERE ID = ?;";
        sqlite3_stmt* stmt;
    
        // 1. Prepare the SQL statement.
        if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL Prepare Error: " << sqlite3_errmsg(DB) << std::endl;
            return;
        }

        // 2. Bind the values to the placeholders.
        sqlite3_bind_int(stmt, 1, id);

        // 3. Execute the statement.
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "SQL Execute Error: " << sqlite3_errmsg(DB) << std::endl;
        } else if (sqlite3_changes(DB) > 0) { // Check if any rows were deleted
            cout << "Task with ID " << id << " deleted successfully." << endl;
        } else {
            cout << "Error: Task with ID " << id << " not found." << endl;
        }

        sqlite3_finalize(stmt);
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