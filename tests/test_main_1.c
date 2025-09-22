#include "../main.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc;(void)argv;(void)envp;

    t_shell_data data;
    data = (t_shell_data){0};

    // Check with empty envp
    // char *envp2[] = { "PATH", NULL };
    // parse_envp(&data, envp2);
    // parse_exec_folders(&data);

    parse_envp(&data, envp);
    if (!data.env_list.first)
    {
        ft_print_err("Error: No environment variables found\n");
        free_shell_data(&data);
        return 1;
    }
    parse_exec_folders(&data);
    if (!data.paths)
    {
        ft_print_err("Error: No PATH variable found in environment\n");
        free_shell_data(&data);
        return 1;
    }
    if (set_envp_from_env(&data))
    {
        ft_print_err("Error: No environment variables found\n");
        free_shell_data(&data);
        return 1;
    }
    
    t_env_node *current;
    current = data.env_list.first;
    int i = 0;
    while (i < data.env_list.len)
    {
        printf("Key and value %s=%s\n", current->key, current->value);
        current = current->next;
        i++;
    }

    printf("\nPATHS:\n");
    i = 0;
    while (data.paths[i])
    {
        printf("Path %s\n", data.paths[i]);
        i++;
    }
    
    printf("\nENV:\n");
    i = 0;
    while (i < data.env_list.len)
    {
        printf("Env %s\n", data.envp[i]);
        i++;
    }

    printf("\nTRYING TO EXECUTE:\n");

    printf("Running ls -la: \n");
    exec_cmd(&data, "ls", "-la");
    printf("Last exit status: %d\n\n", WEXITSTATUS(data.last_exit_status));

    printf("Running gcc --version: \n");
    exec_cmd(&data, "gcc", "--version");
    printf("Last exit status: %d\n\n", WEXITSTATUS(data.last_exit_status));

    printf("Running nonexistentcommand: \n");
    exec_cmd(&data, "nonexistentcommand", "");
    printf("Last exit status: %d\n", WEXITSTATUS(data.last_exit_status));

    free_shell_data(&data);
    return 0;
}