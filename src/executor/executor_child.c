#include "executor.h"

void child_execute(t_shell_data *data, t_cmd_node *cmd, int prev_fd, int pipefd[2])
{
    int     status;

    if (setup_child_fds(cmd, prev_fd, pipefd) < 0)
        exit(1);
    if (!cmd->args)
        exit(1);
    if (is_builtin(cmd->cmd))
    {
        status = exec_builtin(data, cmd->args);
        free_str_arr(cmd->args);
        exit(status);
    }
    exec_external(data, cmd->args);
}

bool should_run_parent_builtin(t_cmd_node *cmd, int exec_count)
{
    if (!cmd || is_empty_cmd(cmd))
        return (false);
    if (!is_builtin(cmd->cmd))
        return (false);
    if (exec_count != 1)
        return (false);
    if (cmd->is_pipe_in || cmd->is_pipe_out)
        return (false);
    return (true);
}
