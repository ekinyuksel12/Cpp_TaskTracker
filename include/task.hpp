#include <ctime>
#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

using namespace std;

enum taskStatus {
    TODO,
    IN_PROGRESS,
    DONE
};

class Task
{
private:
    /* data */
public:
    unsigned int id;
    string description;
    taskStatus status;
    time_t createdAt;
    time_t updatedAt;

    Task (){
        status = taskStatus::TODO;

        time_t currentEpochTime;
        time(&currentEpochTime);

        createdAt = currentEpochTime;
        updatedAt = currentEpochTime;
    }

    void setDescription (string newDescription) {
        description = newDescription;
    }

    void setStatus (taskStatus newStatus) {
        status = newStatus;
    }

    string getStatus () const {
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
};

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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Task, id, description, status, createdAt, updatedAt);