### Unix-Shell

A feature-rich Unix shell implemented in C, capable of handling a wide range of commands and supporting advanced features such as piping, redirection, background execution, and process management. This custom shell allows users to execute single or multiple commands, chain them together using pipes (|), redirect input and output (>, <), and manage processes in the background (&). It also includes error handling, command parsing, and built-in support for common shell commands, providing a robust environment for interacting with the operating system at a low level.

This description emphasizes key functionalities, such as:

- Pipe support (|)
- Redirection (>, <)
- Background process execution (&)
- Error handling
- Command parsing
- Built-in shell commands

### How To Run

```bash
#!/bin/bash

# remove executable
echo "Removing previous executable: rm -f ./a.out"
rm -f ./a.out

# compile
echo "Compiling: gcc -Wall -Wextra *.c"
gcc -Wall -Wextra *.c

# execute
echo "Executing: ./a.out"
./a.out
```
