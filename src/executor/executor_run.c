#include "executor.h"

// parse through all cmd in structure and call for each execution respectively 
static int run_loop(t_shell_data *data, t_exec_ctx *ctx)
{
    while (ctx->current)
    {
        if (is_empty_cmd(ctx->current))
        {
            ctx->current = ctx->current->next;
            continue;
        }
        if (process_command_node(data, ctx, ctx->current))
            return (1);
        ctx->current = ctx->current->next;
    }
    return (0);
}
// start of executuin, initiallaizing structures, call functions 
int execute_command_list(t_shell_data *data)
{
    t_exec_ctx ctx;

    if (!data)
        return (1);
    exec_ctx_setup(&ctx, data);
    if (ctx.exec_count == 0)
    {
        cleanup_heredocs(data->cmd_list.first);
        return (0);
    }
    if (exec_ctx_alloc_pids(&ctx))
        return (1);
    if (run_loop(data, &ctx))
        ctx.had_error = true;
    exec_ctx_finish(data, &ctx);
    return (ctx.had_error);
}
