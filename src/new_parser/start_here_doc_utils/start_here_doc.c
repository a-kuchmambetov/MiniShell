#include "../main.h"
#include "start_here_doc_utils.h"

int delete_here_doc(const char *filename)
{
    if (!filename)
    {
        ft_print_err("error: invalid filename of here-document\n");
        return (1);
    }
    if (unlink(filename) < 0)
    {
        ft_print_err("error: unable to delete here-document temp file\n");
        return (1);
    }
    filename = NULL;
    return (0);
}

static void cleanup_here_doc(char *input, int fd, char **filename)
{
    close(fd);
    if (input)
    {
        free(input);
        input = NULL;
    }
    if (filename && *filename)
    {
        delete_here_doc(*filename);
        free(*filename);
        *filename = NULL;
    }
}

static int write_here_doc(t_env_list env, const char *eof_word, char **filename, int fd)
{
    char *input;

    while (1)
    {
        input = readline("> ");
        if (g_signal_received)
            return (cleanup_here_doc(input, fd, filename), 130);
        if (!input)
            return (close(fd), print_error(eof_word), 0);
        if (compare_eof(input, eof_word))
            break;
        if (eof_word[0] != '\'' && eof_word[ft_strlen(eof_word)] != '\'')
        {
            if (process_expansion(env, &input))
                return (cleanup_here_doc(input, fd, filename), ft_print_err("error: processing expansion failed\n"), 1);
        }
        ft_putendl_fd(input, fd);
        free(input);
    }
    if (input)
        free(input);
    close(fd);
    return (0);
}

int start_here_doc(t_env_list env, const char *eof_word, char **filename)
{
    int fd;
    int result;
    void (*old_handler)(int);

    if (!filename)
        return (1);
    if (*filename)
    {
        free(*filename);
        *filename = NULL;
    }
    g_signal_received = 0;
    rl_done = 0;
    rl_event_hook = check_signal_hook;
    old_handler = signal(SIGINT, handle_sigint_heredoc);
    if (create_file(filename, &fd))
        return (signal(SIGINT, old_handler), 1);
    result = write_here_doc(env, eof_word, filename, fd);
    g_signal_received = 0;
    rl_done = 0;
    rl_event_hook = NULL;
    signal(SIGINT, old_handler);
    return (result);
}
