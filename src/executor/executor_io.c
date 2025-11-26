#include "executor.h"

int redirect_input(t_cmd_node *cmd, int prev_fd)
{
    int fd;

    fd = -1;
    if (!cmd)
        return (-1);
    if (cmd->input_redir_type == REDIR_INPUT || cmd->input_redir_type == REDIR_HEREDOC)
    {
        fd = open(cmd->input_redir, O_RDONLY);
        if (fd < 0)
            return (perror(cmd->input_redir), -1);
        if (dup2(fd, STDIN_FILENO) < 0)
            return (perror("dup2"), close(fd), -1);
        close(fd);
    }
    else if (prev_fd != -1)
    {
        if (dup2(prev_fd, STDIN_FILENO) < 0)
            return (perror("dup2"), -1);
    }
    if (prev_fd != -1)
        close(prev_fd);
    return (0);
}

int redirect_output(t_cmd_node *cmd, int pipefd[2])
{
    int fd;
    int flags;

    fd = -1;
    if (cmd->output_redir_type == REDIR_OUTPUT || cmd->output_redir_type == REDIR_APPEND)
    {
        flags = O_WRONLY | O_CREAT;
        if (cmd->output_redir_type == REDIR_OUTPUT)
            flags |= O_TRUNC;
        else
            flags |= O_APPEND;
        fd = open(cmd->output_redir, flags, 0644);
        if (fd < 0)
            return (perror(cmd->output_redir), -1);
        if (dup2(fd, STDOUT_FILENO) < 0)
            return (perror("dup2"), close(fd), -1);
        close(fd);
        if (pipefd[1] != -1)
            close(pipefd[1]);
    }
    else if (pipefd[1] != -1)
    {
        if (dup2(pipefd[1], STDOUT_FILENO) < 0)
            return (perror("dup2"), -1);
        close(pipefd[1]);
    }
    if (pipefd[0] != -1)
        close(pipefd[0]);
    return (0);
}

int setup_child_fds(t_cmd_node *cmd, int prev_fd, int pipefd[2])
{
    if (redirect_input(cmd, prev_fd) < 0)
        return (-1);
    if (redirect_output(cmd, pipefd) < 0)
        return (-1);
    return (0);
}
