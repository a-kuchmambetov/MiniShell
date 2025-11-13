#include "executor.h"

static int prepare_pipe(int pipefd[2], t_cmd_node *cmd)
{
    pipefd[0] = -1;
    pipefd[1] = -1;
    if (!cmd->is_pipe_out)
        return (0);
    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        return (1);
    }
    return (0);
}

static int handle_iter_error(t_exec_ctx *ctx, int pipefd[2], int input_fd)
{
    if (pipefd[0] != -1)
        close(pipefd[0]);
    if (pipefd[1] != -1)
        close(pipefd[1]);
    if (input_fd != -1)
        close(input_fd);
    ctx->prev_pipe_read = -1;
    ctx->had_error = true;
    return (1);
}

int spawn_child_process(t_shell_data *data, t_exec_ctx *ctx,
        t_cmd_node *cmd, int pipefd[2])
{
    ctx->pids[ctx->child_count] = fork();
    if (ctx->pids[ctx->child_count] == 0)
        child_execute(data, cmd, ctx->prev_pipe_read, pipefd);
    if (ctx->pids[ctx->child_count] < 0)
    {
        perror("fork");
        return (1);
    }
    if (cmd == ctx->last_exec)
        ctx->last_child = ctx->pids[ctx->child_count];
    ctx->child_count++;
    return (0);
}

static void update_pipe_links(t_exec_ctx *ctx, t_cmd_node *cmd, int pipefd[2])
{
    if (cmd->is_pipe_out)
    {
        if (pipefd[1] != -1)
            close(pipefd[1]);
        ctx->prev_pipe_read = pipefd[0];
    }
    else
    {
        if (pipefd[0] != -1)
            close(pipefd[0]);
        if (pipefd[1] != -1)
            close(pipefd[1]);
        ctx->prev_pipe_read = -1;
    }
}

int process_command_node(t_shell_data *data, t_exec_ctx *ctx, t_cmd_node *cmd)
{
    int pipefd[2];
    int input_fd;
    int parent_run;

    if (prepare_pipe(pipefd, cmd))
        return (handle_iter_error(ctx, pipefd, ctx->prev_pipe_read));
    input_fd = ctx->prev_pipe_read;
    parent_run = should_run_parent_builtin(cmd, ctx->exec_count);
    if (parent_run && run_parent_builtin(data, ctx, cmd))
        return (handle_iter_error(ctx, pipefd, input_fd));
    if (!parent_run && spawn_child_process(data, ctx, cmd, pipefd))
        return (handle_iter_error(ctx, pipefd, input_fd));
    if (input_fd != -1)
        close(input_fd);
    update_pipe_links(ctx, cmd, pipefd);
    return (0);
}
