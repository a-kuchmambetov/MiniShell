#include "executor.h"

void exec_ctx_setup(t_exec_ctx *ctx, t_shell_data *data)
{
    ctx->current = data->cmd_list.first;
    ctx->last_exec = find_last_executable(ctx->current);
    ctx->exec_count = count_executable(ctx->current);
    ctx->pids = NULL;
    ctx->child_count = 0;
    ctx->last_child = -1;
    ctx->prev_pipe_read = -1;
    ctx->parent_handled_last = false;
    ctx->status_updated = false;
    ctx->had_error = false;
}

int exec_ctx_alloc_pids(t_exec_ctx *ctx)
{
    if (ctx->exec_count <= 0)
        return (0);
    ctx->pids = ft_calloc(sizeof(pid_t), ctx->exec_count);
    if (!ctx->pids)
    {
        ft_print_err("Memory allocation error\n");
        return (1);
    }
    return (0);
}

void exec_ctx_finish(t_shell_data *data, t_exec_ctx *ctx)
{
    if (ctx->prev_pipe_read != -1)
    {
        close(ctx->prev_pipe_read);
        ctx->prev_pipe_read = -1;
    }
    if (!ctx->parent_handled_last && ctx->child_count > 0
        && ctx->last_child != -1)
    {
        update_last_exit_status(data,
            wait_children(ctx->pids, ctx->child_count, ctx->last_child));
        ctx->status_updated = true;
    }
    else
        wait_children(ctx->pids, ctx->child_count, ctx->last_child);
    free(ctx->pids);
    cleanup_heredocs(data->cmd_list.first);
    if (ctx->had_error && !ctx->status_updated)
        update_last_exit_status(data, 1);
}
