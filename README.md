 Nano-Shell 
**A lightweight, custom Unix-like command-line interpreter built from scratch in C.**

Nano-Shell is a systems engineering project designed to demonstrate a deep understanding of core Operating System architectures. It interfaces directly with the Linux kernel via POSIX system calls to manage process lifecycles, handle asynchronous signals, and establish inter-process communication (IPC).

# Core Architecture & Features

This shell is not a wrapper; it executes system binaries natively using a custom-built Read-Eval-Print Loop (REPL) pipeline.

* **Process Management:** Utilizes `fork()`, `execvp()`, and `wait()` to seamlessly clone processes, execute system binaries (like `ls`, `grep`, `pwd`), and synchronize parent-child execution states.
* **Environment Control:** Implements built-in commands (like `cd` and `exit`) that modify the parent shell's memory and working directory directly using `chdir()`, bypassing standard child-process isolation.
* **Concurrency & Signal Handling:** * Supports asynchronous **background processing** using the `&` operator.
    * Prevents terminal crashes by intercepting `SIGINT` (Ctrl+C).
    * Implements a custom "Zombie Reaper" via the `SIGCHLD` signal and `waitpid(WNOHANG)` to automatically clean up orphaned background processes and prevent memory leaks.
* **Inter-Process Communication (IPC):** * **Piping (`|`):** Plumbs file descriptors across concurrent child processes using `pipe()` and `dup2()`, allowing the output of one command to flow directly into the input of another.
    * **Redirection (`>`):** Hijacks standard output (`STDOUT_FILENO`) to route execution results directly to system files using `open()` and kernel-level file descriptor manipulation.

# Technical Stack
* **Language:** C 
* **API:** POSIX (Linux/Unix Standard Library)
* **Key System Calls:** `fork`, `execvp`, `wait`, `waitpid`, `pipe`, `dup2`, `chdir`, `signal`, `getcwd`

#Installation & Usage

**Prerequisites:** A Unix-like operating system (Linux, macOS, or Windows WSL) and the GCC compiler.

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/Parth2024P/nano-shell.git](https://github.com/Parth2024P/nano-shell.git)
   cd nano-shell
