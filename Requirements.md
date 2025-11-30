# Task Tracker CLI - Requirements Document (v2.0)

# 1. Project Overview
Task Tracker is a lightweight, command-line interface (CLI) application designed to help users track their daily tasks and to-dos using a robust local database.

# 2. Goals & Scope

### 2.1. Functional Goals
The application must support full CRUD (Create, Read, Update, Delete) operations for tasks:
- Add a new task with a description.
- List all existing tasks.
- Update the description or status of an existing task.
- Mark a task as completed.
- Delete a task from the tracker.

### 2.2. Non-Functional Goals
- **Lightweight:** Minimal external dependencies (standard C++ libraries + SQLite3).
- **Cross-Platform:** The code must be compilable and runnable on standard systems (Linux, macOS, Windows).
- **Persistence:** Data must be saved locally in a relational database file (`tasks.db`).
- **Security:** All database interactions must use **Prepared Statements** to prevent SQL injection.

# 3. User Stories & Features

| Feature | Description | Priority |
| :--- | :--- | :--- |
| **Add Task** | As a user, I want to add a new task so I can keep track of my work. | High |
| **List Tasks** | As a user, I want to see all my tasks with their readable status (e.g., "Pending", "Done") to know what to do. | High |
| **Update Task** | As a user, I want to change a task's description in case requirements change. | High |
| **Mark Done** | As a user, I want to mark a task as "completed" to track my progress. | High |
| **Delete Task** | As a user, I want to remove tasks that are no longer needed. | High |

# 4. Technical Specifications

## 4.1. Tech Stack
- **Language:** C++ (Standard C++17 or later).
- **Compiler:** g++ / clang++ / MSVC.
- **Database Engine:** SQLite 3 (Native C API).
- **Storage:** Local binary file (`tasks.db`).

## 4.2. Database Schema (Data Model)
The data will be normalized into two relational tables to ensure data integrity.

### Table 1: `status` (Lookup Table)
Used to enforce valid task states.
| Column | Type | Constraints | Description |
| :--- | :--- | :--- | :--- |
| `id` | INTEGER | PRIMARY KEY | 1=Pending, 2=In Progress, 3=Done |
| `name` | TEXT | NOT NULL, UNIQUE | Readable name of the status |

### Table 2: `tasks` (Main Table)
| Column | Type | Constraints | Description |
| :--- | :--- | :--- | :--- |
| `id` | INTEGER | PRIMARY KEY AUTOINCREMENT | Unique ID for the task |
| `description` | TEXT | NOT NULL | Details of the task |
| `status_id` | INTEGER | NOT NULL, DEFAULT 1 | Foreign Key referencing `status(id)` |
| `created_at` | INTEGER | NOT NULL | Unix Timestamp of creation |
| `updated_at` | INTEGER | NOT NULL | Unix Timestamp of last modification |

# 5. CLI Interface Design

The application will be invoked via the command line using flags.

## 5.1. Commands
| Action | Flag / Command | Argument | Example |
| :--- | :--- | :--- | :--- |
| **Add** | `-a`, `--add` | description | `./task-cli -a "Buy groceries"` |
| **List** | `-l`, `--list` | (None) | `./task-cli -l` |
| **Update** | `-u`, `--update` | id | `./task-cli -u 1 --name "New Name"` |
| **Complete** | `-c`, `--complete` | id | `./task-cli -c 1` |
| **Delete** | `-d`, `--delete` | id | `./task-cli -d 1` |
| **Help** | `-h`, `--help` | (None) | `./task-cli --help` |

# 6. Future Roadmap

These features are planned for future iterations (v3.0):

- **Complex Filtering:** Filter list by status (e.g., `task-cli --list --status done`) or date range using SQL `WHERE` clauses.
- **Backup & Restore:** CLI command to export the `tasks.db` to a `.sql` dump file for backup.
- **Interactive Mode:** A TUI (Text User Interface) using a library like `ncurses` or `ftxui` to navigate tasks without repeated commands.