#include "../main.h"

static char	*check_do_expansion(t_env_list env, char *input, int *errno,
		int *current_type)
{
	char	*result;

	if (!input)
	{
		*errno = 1;
		return (NULL);
	}
	if (input[0] == '\'')
		return (NULL);
	result = process_expansion(env, input, errno, current_type);
	if (*current_type == TOKEN_TEXT && result)
		*current_type = TOKEN_EXPANSION;
	return (result);
}

void test(t_shell_data data, char *input)
{
    char *s = ft_strdup(input);
    char *res;
    int errno = 0;
    int current_type = TOKEN_TEXT;

    res = check_do_expansion(data.env_list, input, &errno, &current_type);
    if (res == NULL)
        ft_printf("\nNo expansion for (%s)\n", input);
    else
    ft_printf("\nExpanded (%s): %s\n", input,res);
    
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
    init_shell_data(&data, envp);

    test(data, "$HOME");
    test(data, "$PATH");
    test(data, "$_");
    test(data, "$$");
    test(data, "$?");
    test(data, "$UNDEFINED_VAR");
    test(data, "'This is $HOME'");
    test(data, "\"This is $HOME\"");
    test(data, "\"Mixing 'quotes $HOME' and $HOME\"");
    test(data, "$HOME and $PATH");
    test(data, "$TEMP$HOME and more text");

    free_shell_data(&data);
    return (data.last_exit_status);
}