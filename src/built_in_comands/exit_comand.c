#include "builtins.h"

int builtin_exit(t_shell_data *data)
{
    if (data)
    {
        free_shell_data(data);
        free(data);
    }
    rl_clear_history();
    exit(0);
}