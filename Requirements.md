# Task Tracker CLI - Requirements Document

# 1. Project Overview

Task Tracker is a lightweight, command-line interface (CLI) application designed to help users track their daily tasks and to-dos.

# 2. Goals & Scope

### 2.1. Functional Goals

The application must support full CRUD (Create, Read, Update, Delete) operations for tasks:

- Add a new task with a description.

- List all existing tasks.

- Update the description of an existing task.

- Mark a task as completed.

- Delete a task from the tracker.

### 2.2. Non-Functional Goals

- Lightweight: Minimal external dependencies.

- Cross-Platform: The code must be compilable and runnable on any system with a standard C++ compiler (Linux, macOS, Windows).

- Persistence: Data must be saved locally between sessions.

## 3. User Stories & Features

| Feature     	| Description                                                                                     	| Priority 	|
|-------------	|-------------------------------------------------------------------------------------------------	|----------	|
| Add Task    	| As a user, I want to add a new task so I can keep track of my work.                             	| High     	|
| List Tasks  	| As a user, I want to see all my tasks (ID, status, description, timestamps) to know what to do. 	| High     	|
| Update Task 	| As a user, I want to change a task's description in case requirements change.                   	| High     	|
| Mark Done   	| As a user, I want to mark a task as "completed" to track my progress.                           	| High     	|
| Delete Task 	| As a user, I want to remove tasks that are no longer needed.                                    	| High     	|

# 4. Technical Specifications

## 4.1. Tech Stack

- Language: C++ (Standard C++17 or later recommended).

- Direct compiler usage (g++/clang++).

- Storage: Local JSON file (db.json).

## 4.2. Data Model

Each task object in db.json should contain:
- id: Unique integer identifier (auto-incrementing).

- description: String details of the task.

- status: Enum/String (Todo, In Progress, Done).

- createdAt: Timestamp of creation.

- updatedAt: Timestamp of last modification.

# 5. CLI Interface Design

The application will be invoked via the command line using flags.

## 5.1. Commands
| Action   	| Flag / Command 	| Argument           	| Example                                 	|
|----------	|----------------	|--------------------	|-----------------------------------------	|
| Add      	| -a, --add      	| description      	| ./task-cli -a "Buy groceries"           	|
| List     	| -l, --list     	| (None)             	| ./task-cli -l                           	|
| Update   	| -u, --update   	| id, description 	| ./task-cli -u 1 "Buy organic groceries" 	|
| Complete 	| -c, --complete 	| id               	| ./task-cli -c 1                         	|
| Delete   	| -d, --delete   	| id               	| ./task-cli -d 1                         	|
| Help     	| -h, --help     	| (None)             	| ./task-cli --help                       	|

**Note on Update/Delete:** The user must provide the Task ID to perform these actions.

# 6. Future Roadmap

These features are planned for future iterations (v2.0):

- Database Migration: Migrate storage from db.json to a local SQLite database for better scalability and query capabilities.

- Status Filters: Add filtering to the list command (e.g., list only "done" tasks).

- Due Dates: Allow setting deadlines for tasks.