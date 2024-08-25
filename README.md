# 42Minishell

A recreation of the bash shell using the GNU readline library.

When run, minishell will display a prompt showing the username, hostname, and current directory. The program will then wait for user input. The program will then run an infinite loop of displaying a prompt before processing and running the user's input.

#### The shell implements the following features:

- A working history using the up and down arrow keys
- First searches for a relative or absolute path for a command, otherwise, searches the path environment variable
- Handles `'` and `"` which prevents meta characters from being interpreted except `$` when using `"`
- Handles the 4 redirects (`<`, `>`, `<<`, `>>`)
- Handles piping commands (`|`)
- Uses `$(val)` to expand environment variables in commands
- Expands `$?` to the return value of the last run program
- Handles the `ctrl-C`, `ctrl-D`, and `ctrl-\` identical to bash
- Implements the builtins: echo, cd, pwd, export, unset, env, and exit

## Restrictions

42 imposes a strict set of restrictions that must be followed. The program must be built with the flags `-Wall -Wextra -Werror`, and all build errors or warnings will mark the project as a failure. There is also a list of allowed functions and a style guide that must be followed exactly. 

### Allowed functions

Only functions from the list below can be used or the project is marked as a failure:

readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs.

### Notable style restrictions

An assignment will be marked as a failure if the style guide is not followed exactly and there is a program to flag errors. The full style guide can be viewed [here](https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf), however below are some notable restrictions:
- Only 25 lines are allowed in any function
- Only 5 functions are allowed in each file
- Only 4 arguments are allowed for each function
- Lines must contain 80 characters or less
- The do..while, for, ternary, case..switch, and GOTO operators are forbidden
- Curly brackets must be on the line following a statement

## Build

To build this code, clone the repository and run the `make` command in the root directory. It will use Clang to build the project and output the file `minishell`.

## Run

To run this program, use the following syntax:

```./minishell```

If the program isn't run in an interactive shell, neither the splash screen nor the prompt will be displayed, and the program will execute each command line by line.

## Tester

For this project I made a custom class structure to make unit testing easier to visualize and debug. 

Below is an example of how the tester looks with memory leaks, crashes, and regular failed tests

<img width="1270" alt="Screenshot 2024-08-06 at 12 45 20 PM" src="https://github.com/user-attachments/assets/d9edc2c0-5d0e-4019-aa6c-f7504decc68d">


