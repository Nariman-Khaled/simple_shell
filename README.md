# Simple Shell Project

Welcome to our Simple Shell project, a collaborative effort for the ALX Software Engineering Program. This project aims to build a basic shell interface that interprets commands to interact with the operating system, utilizing Bash, C, and Assembly languages. It serves as a hands-on introduction to system programming, exploring low-level interaction, process management, and the essentials of creating a user-friendly command-line environment.

## Project Description

The Simple Shell project is an educational journey into the workings of command-line interfaces similar to the Unix shell. It emphasizes the practical application of C programming skills, Bash scripting, and Assembly language to create a functional shell that can execute commands, manage user inputs, and interact with the kernel at a basic level.

## Features

- **Command Execution**: The ability to run commands typed by the user or from a script.
- **Scripting Capabilities**: Executes simple Bash scripts to automate tasks.
- **Environment Variable Management**: Handles the creation, modification, and deletion of environment variables.
- **Custom Built-in Commands**: Includes a set of built-in commands implemented from scratch.

## Getting Started

### Prerequisites

Before you begin, ensure you have the following installed on your system:
- GCC (GNU Compiler Collection) for compiling C programs.
- NASM (The Netwide Assembler) for assembling your Assembly code.
- A Linux or Unix-like operating system for development and testing.

### Installation

1. **Clone the Repository**: First, clone this repository to your local machine using:

    ```
    git clone https://github.com/Nariman-Khaled/simple_shell.git
    ```

2. **Navigate to the Project Directory**:

    ```
    cd simple_shell
    ```

3. **Compile the Program**: Use the provided Makefile or compile manually with:

    ```
    gcc -o simple_shell *.c -Wall -Werror -Wextra -pedantic
    ```

### Usage

To start the shell after compilation, run:

