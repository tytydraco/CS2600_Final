#pragma once

/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
static char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

static int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

#define lsh_num_builtins() sizeof(builtin_str) / sizeof(char *)