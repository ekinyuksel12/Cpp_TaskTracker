#include <ctime>
#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

using namespace std;

//An enumarator to hold task's status.
//TODO means task is created but not currently worked on.
//IN_PROGRESS means task is currently being worked on.
//DONE means task is completed.
enum taskStatus {
    TODO,
    IN_PROGRESS,
    DONE
};

class Task
{
public:
    unsigned int id;
    string description;
    taskStatus status;
    time_t createdAt;
    time_t updatedAt;

    Task (){
        status = taskStatus::TODO;
        
        //Get current epoch time to set createdAt and updatedAt to the same exact time.
        //We do this to make sure createdAt and updatedAt is same in the begining.
        time_t currentEpochTime;
        time(&currentEpochTime);

        createdAt = currentEpochTime;
        updatedAt = currentEpochTime;
    }
    
    void setDescription (string newDescription);
    void setStatus (taskStatus newStatus);

    string getStatus () const;
};

//Setter and Getter Functions
void Task::setDescription (string newDescription) {
    description = newDescription;
}

void Task::setStatus (taskStatus newStatus) {
    status = newStatus;
}

//Convert TaskStatus enum to strings.
string Task::getStatus () const {
    switch (status)
    {
    case taskStatus::TODO:
        return "to-do";
        break;
    
    case taskStatus::IN_PROGRESS:
        return "in_progress";
        break;
    
    case taskStatus::DONE:
        return "done";
        break;
    
    default:
        break;
    }
    return "";
}

//Overloading the '<<' operator for printing the task object easily.
//The current format is 'ID: 0 (to-do) - Hello World [Thu Sep 18 21:16:51 2025 | Thu Sep 18 21:16:51 2025]'
ostream& operator<<(ostream& os, const Task& task) {
    auto format_time = [](time_t timeValue) -> string {
        char* c_time_string = std::ctime(&timeValue);
        
        // Convert to a C++ string to easily remove the trailing newline
        std::string time_str(c_time_string);
        
        // Remove the newline character that ctime adds
        if (!time_str.empty() && time_str.back() == '\n') {
            time_str.pop_back();
        }
        return time_str;
    };

    os << "ID: " << task.id << " (" << task.getStatus() << ") - " << task.description << " [" << format_time(task.createdAt) << " | " << format_time(task.updatedAt) << "] ";
    return os;
}

//This macro creates to_json and from_json functions in order to save object to json directly.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Task, id, description, status, createdAt, updatedAt);