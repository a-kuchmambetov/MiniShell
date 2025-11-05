#include "../main.h"

void test(t_shell_data data, char *input)
{
    char *s = ft_strdup(input);
    char **res = &s;
    check_do_expansion(data.env_list, res);
    ft_printf("\nExpanded (%s): %s\n", input,*res);
    free(s);
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;

    t_shell_data data;
    data = (t_shell_data){0};

    // Check with empty envp
    // char *envp2[] = {"PATH=/home/artem/.nvm/versions/node/v22.19.0/bin:/home/artem/.local/funcheck/host:/home/artem/.npm-global/bin:/home/artem/bin:/home/artem/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin", "HOME=/home/artem", NULL};
    // parse_envp(&data, envp2);
    parse_envp(&data, envp);
    set_envp_from_env(&data);
    parse_exec_folders(&data);

    test(data, "$HOME");
    test(data, "$PATH");
    test(data, "$_");
    test(data, "$$");
    test(data, "$?");
    test(data, "$UNDEFINED_VAR");
    test(data, "'This is $HOME'");
    test(data, "\"This is $HOME\"");
    test(data, "\"Mixing 'quotes' and $HOME\"");
    test(data, "$HOME and $PATH");
    test(data, "$TEMP$HOME and more text");

    free_shell_data(&data);
    return (data.last_exit_status);
}