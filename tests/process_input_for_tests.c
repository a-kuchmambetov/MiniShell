#include "../main.h"

void process_input(t_shell_data *data, t_cmd_node *cmd_data)
{
    char **args = (char *[]){cmd_data->cmd, cmd_data->args, NULL};

    if (!args)
        return;
    if (ft_strncmp(cmd_data->cmd, "$?", 2) == 0)
    {
        ft_printf("%d: command not found\n", WEXITSTATUS(data->last_exit_status));
        return;
    }
    if (is_builtin(cmd_data->cmd))
        data->last_exit_status = exec_builtin(data, args);
    else
        exec_cmd(data, cmd_data->cmd, args);
}
