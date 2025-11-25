#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "main.h"
#include <stdbool.h>

typedef struct s_exec_ctx
{
    t_cmd_node *current;
    t_cmd_node *last_exec;
    pid_t *pids;
    int exec_count;
    int child_count;
    pid_t last_child;
    int prev_pipe_read;
    bool parent_handled_last;
    bool status_updated;
    bool had_error;
} t_exec_ctx;

typedef struct s_arg_state
{
    const char *src;
    size_t i;
    size_t start;
    size_t count;
    char **arr;
} t_arg_state;

bool is_empty_cmd(t_cmd_node *cmd);
bool is_question_command(t_cmd_node *cmd);
int count_executable(t_cmd_node *node);
t_cmd_node *find_last_executable(t_cmd_node *node);
void cleanup_heredocs(t_cmd_node *cmd);

char **split_args(const char *args, int *errored);
char **build_argv(t_cmd_node *cmd);

int redirect_input(t_cmd_node *cmd, int prev_fd);
int redirect_output(t_cmd_node *cmd, int pipefd[2]);
int setup_child_fds(t_cmd_node *cmd, int prev_fd, int pipefd[2]);

void exec_external(t_shell_data *data, char **argv);
void child_execute(t_shell_data *data, t_cmd_node *cmd,
                   int prev_fd, int pipefd[2]);
bool should_run_parent_builtin(t_cmd_node *cmd, int exec_count);
int execute_builtin_parent(t_shell_data *data, t_cmd_node *cmd);

int wait_children(pid_t *pids, int count, pid_t last_child);

void exec_ctx_setup(t_exec_ctx *ctx, t_shell_data *data);
int exec_ctx_alloc_pids(t_exec_ctx *ctx);
void exec_ctx_finish(t_shell_data *data, t_exec_ctx *ctx);

int run_parent_builtin(t_shell_data *data, t_exec_ctx *ctx,
                       t_cmd_node *cmd);
int spawn_child_process(t_shell_data *data, t_exec_ctx *ctx,
                        t_cmd_node *cmd, int pipefd[2]);
int process_command_node(t_shell_data *data, t_exec_ctx *ctx,
                         t_cmd_node *cmd);

int execute_command_list(t_shell_data *data);

#endif
