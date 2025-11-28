# Cpp_TaskTracker
A simple command-line task tracker application built with C++.

## Features
- Add new tasks with descriptions.
- View a list of all current tasks.
- Mark tasks as completed.
- Update a task's description.
- Delete tasks from the list.
- Persists tasks to a local file (`db.json`).

---

## Getting Started

### Prerequisites

- A C++ compiler that supports **C++17** (e.g., GCC, Clang, MSVC).

### Building

1.  **Clone the repository:**
    ```sh
    git clone [https://github.com/your-username/Cpp_TaskTracker.git](https://github.com/your-username/Cpp_TaskTracker.git)
    cd Cpp_TaskTracker
    ```

2.  **Compile the source code:**
    
    Since this project uses header-only libraries, you can compile it directly using your C++ compiler.
    
    Assuming your source file is `task-cli.cpp` and you want the executable named `TaskTracker`:
    
    ```sh
    # Use -std=c++17 for required features
    # Adjust paths if header files (like cxxopts.hpp, nlohmann/json.hpp, task.hpp) 
    # are not in standard include locations.
    g++ -std=c++17 task-cli.cpp -o TaskTracker
    ```

---

## Usage

After a successful build, the executable will be located in the same directory where you ran the compilation command.

The application is run using the executable followed by a command flag. Run with `--help` for a full list of options:

```sh
./TaskTracker --help
````

### Commands

  * **List all tasks** (`--list` or `-l`)

    ```sh
    ./TaskTracker --list
    ```

  * **Add a new task** (`--add` or `-a`)

    ```sh
    ./TaskTracker --add "Task description here"
    ```

  * **Delete a task** (`--delete` or `-d`)

    ```sh
    ./TaskTracker --delete 3
    ```

  * **Mark a task as completed** (`--complete` or `-c`)

    ```sh
    ./TaskTracker --complete 1
    ```

  * **Update a task's description** (`--update` or `-u`)
    (Requires the task ID and the new description with `--name`)

    ```sh
    ./TaskTracker --update 2 --name "New updated task description"
    ```

-----

## Contributing

Contributions are welcome. Please open an issue to discuss your ideas or submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.