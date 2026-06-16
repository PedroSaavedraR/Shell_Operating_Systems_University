# Shell Operating Systems - University Assignment

A practical assignment for understanding operating systems concepts through shell implementation and system programming.

## Overview

This project demonstrates core operating systems principles including process management, system calls, and shell implementation. Students work on building a functional shell that interacts with the OS kernel.

## Learning Objectives

- Understand process creation and management
- Learn about system calls and their use
- Implement command parsing and execution
- Work with file descriptors and redirection
- Handle process synchronization and signals

---

## Available Commands

### **System Information**

#### `authors`
Prints the names and logins of the program authors.
- `authors` - Shows full names and logins
- `authors -l` - Shows only logins
- `authors -n` - Shows only names

#### `pid`
Prints the PID (Process ID) of the shell process.

#### `ppid`
Prints the PPID (Parent Process ID) of the shell.

#### `date`
Displays current date and time.
- `date` - Shows date (DD/MM/YYYY) and time (hh:mm:ss)
- `date -d` - Shows only the date
- `date -t` - Shows only the time

#### `infosys`
Prints system information obtained via `uname` system call (system name, machine, node name, version, release).

#### `help`
Displays available commands or detailed help for a specific command.
- `help` - Lists all available commands
- `help <command>` - Shows help for specific command

---

### **Directory Management**

#### `cd <directory>`
Changes the current working directory (uses `chdir` system call).
- `cd` - Without arguments, displays current working directory
- `cd <path>` - Changes to specified directory

#### `cwd`
Prints the current working directory.

#### `makedir <directory_name>`
Creates a new directory with rwx permissions for owner and rx for others.

#### `makefile <filename>`
Creates a new file with read/write permissions for all users.

#### `listdir [-hid] [-long] [-link] [-acc] <directory>`
Lists directory contents with optional formatting flags.
- `-hid` - Include hidden files
- `-long` - Show long format with ownership and permissions
- `-link` - Display symbolic link targets
- `-acc` - Show access times

#### `listfile [-long] [-acc] [-link] <file>`
Displays detailed information about specific files.
- `-long` - Show long listing (hardlinks, inode, user, group, permissions)
- `-acc` - Show access time
- `-link` - Show symbolic link target

#### `reclist [-hid] [-long] [-link] [-acc] <directory>`
Lists directories recursively with subdirectories shown after parent content.

#### `revlist [-hid] [-long] [-link] [-acc] <directory>`
Lists directories recursively with subdirectories shown before parent content.

#### `erase <file_or_directory>`
Deletes files and empty directories.

#### `delrec <directory>`
Recursively deletes a directory and all its contents.

---

### **File Operations**

#### `open <filename> [cr] [ex] [ro] [wo] [rw] [ap] [tr]`
Opens a file and adds it to the shell's open file list.
- `cr` - Create file if it doesn't exist
- `ex` - Exclusive access
- `ro` - Read-only
- `wo` - Write-only
- `rw` - Read/Write
- `ap` - Append mode
- `tr` - Truncate file
- No arguments - Lists all open files

#### `close <file_descriptor>`
Closes a file descriptor and removes it from the open file list.
- No arguments - Lists open files

#### `dup <file_descriptor>`
Duplicates a file descriptor using the `dup` system call.

---

### **Memory Management**

#### `allocate [-malloc|-shared|-createshared|-mmap] [parameters]`
Allocates memory blocks using various methods.
- `allocate -malloc <size>` - Allocate memory via malloc
- `allocate -createshared <key> <size>` - Create shared memory with key and size
- `allocate -shared <key>` - Attach to existing shared memory
- `allocate -mmap <filename> <permissions>` - Memory-map a file (permissions: r/w/x)
- No arguments - Lists all allocated memory blocks

#### `deallocate [-malloc|-shared|-delkey|-mmap] [parameters]`
Deallocates memory blocks.
- `deallocate -malloc <size>` - Free malloc block of size
- `deallocate -shared <key>` - Detach shared memory
- `deallocate -delkey <key>` - Delete shared memory from system
- `deallocate -mmap <filename>` - Unmap file
- `deallocate <address>` - Free memory at specific address

#### `memfill <address> [size] [character]`
Fills memory with a character starting at address for specified bytes.
- Default size: 128 bytes
- Default character: 'a'

#### `memdump <address> [size]`
Displays memory contents in hexadecimal and ASCII format.
- Default size: 128 bytes

#### `readfile <filename> <memory_address> [size]`
Reads file contents into memory at specified address.

#### `writefile <filename> <memory_address> [size]`
Writes memory contents to a file.

#### `read <file_descriptor> <address> [size]`
Reads from file descriptor into memory address.

#### `write <file_descriptor> <address> <size>`
Writes from memory address to file descriptor.

#### `memory [-funcs|-vars|-pmap|-blocks|-all]`
Displays memory-related information.
- `-funcs` - Shows addresses of 3 program and 3 library functions
- `-vars` - Shows addresses of local, global, and static variables
- `-pmap` - Executes pmap/procstat/vmmap to show process memory map
- `-blocks` - Lists allocated memory blocks
- `-all` - Shows everything

#### `recurse <n>`
Executes a recursive function `n` times, displaying stack addresses for automatic and static arrays.

---

### **Process Management**

#### `fork`
Creates a child process using `fork` system call and waits for it to complete.

#### `fg <program> [arguments]`
Executes a program in the foreground, waits for completion, and displays exit status.

#### `fgpri <priority> <program> [arguments]`
Executes a program in the foreground with a specified priority.

#### `back <program> [arguments]`
Executes a program in the background.

#### `backpri <priority> <program> [arguments]`
Executes a program in the background with a specified priority.

#### `listjobs`
Lists all background processes currently running (PID, owner, priority, creation date, command).

#### `deljob [-term|-sig]`
Terminates or signals a background process.

#### `exec <program> [arguments]`
Executes a program without creating a new process (replaces current shell).

#### `search [-add|-del|-clear|-path]`
Manages the executable search list (directories where shell looks for programs).
- `search` - Lists current directories
- `search -add <directory>` - Adds directory to search list
- `search -del <directory>` - Removes directory from search list
- `search -clear` - Clears entire search list
- `search -path` - Imports directories from PATH environment variable

---

### **Environment & Credentials**

#### `getuid`
Displays real and effective user IDs of the current process.

#### `setuid <uid>`
Changes the effective UID of the process.
- `setuid -l` - Shows current credentials

#### `showvar <variable_names>`
Displays environment variable values and memory addresses accessed via `environ`, `getenv()`, and main() arguments.

#### `changevar [-a|-e|-p] <variable> <value>`
Modifies environment variables.
- `-a` - Add or modify (direct environment modification)
- `-e` - Modify if exists (direct environment modification)
- `-p` - Use putenv() to modify

#### `subsvar [-a|-e] <variable1> <variable2> <value>`
Substitutes one environment variable with another.
- `-a` - Always substitute
- `-e` - Substitute only if variable exists

#### `environ [-addr|-environ]`
Displays process environment.
- No arguments - Shows all environment variables
- `-addr` - Shows address of environ pointer
- `-environ` - Shows indexed environ array

---

### **Shell Control**

#### `historic [N] [-N]`
Shows command history or executes a previous command.
- `historic` - Displays all commands with index numbers
- `historic <N>` - Executes command number N
- `historic -<N>` - Shows last N commands

#### `quit` / `exit` / `bye`
Terminates the shell and frees all allocated resources.

---

## License

MIT License - See LICENSE file for details
