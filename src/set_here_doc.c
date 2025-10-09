#include "../main.h"

static void handle_sigint_heredoc(int sig)
{
    (void)sig;
    g_signal_received = 1;
    rl_done = 1;
    write(STDOUT_FILENO, "\n", 1);
}

int check_file_name(char **name)
{
    const char *name_template = HERE_DOC_DIR HERE_DOC_PREFIX;
    int counter;
    char *counter_str;
    char *new_name;

    counter = 0;
    *name = NULL;
    while (1)
    {
        counter_str = ft_itoa(counter);
        if (!counter_str)
            return (free(counter_str), 1);
        new_name = ft_strjoin(name_template, counter_str);
        free(counter_str);
        if (!new_name)
            return (1);
        if (access(new_name, F_OK) != 0)
        {
            *name = new_name;
            return (0);
        }
        free(new_name);
        counter++;
    }
    return (0);
}

int create_file(char **filename, int *fd)
{
    if (check_file_name(filename))
        return (1);
    *fd = open(*filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (*fd < 0)
    {
        ft_print_err("Error: Unable to create here-document temp file\n");
        return (1);
    }
    return (0);
}

int delete_here_doc(const char *filename)
{
    if (!filename)
    {
        ft_print_err("Error: Invalid filename of here-document\n");
        return (1);
    }
    if (unlink(filename) < 0)
    {
        ft_print_err("Error: Unable to delete here-document temp file\n");
        return (1);
    }
    filename = NULL;
    return (0);
}

static void cleanup_here_doc(char *input, int fd, char **filename)
{
    if (input)
        free(input);
    close(fd);
    if (filename && *filename)
    {
        delete_here_doc(*filename);
        free(*filename);
        *filename = NULL;
    }
}

int write_here_doc(const char *eof_word, char **filename, int fd)
{
    char *input;

    while (1)
    {
        input = readline("> ");
        if (g_signal_received)
            return (cleanup_here_doc(input, fd, filename), 130); // SIGINT exit code
        if (!input)
        {
            cleanup_here_doc(NULL, fd, filename);
            break;
        }
        if (ft_strncmp(input, eof_word, ft_strlen(eof_word)) == 0)
        {
            free(input);
            break;
        }
        process_expansion(&input);
        write(fd, input, ft_strlen(input));
        write(fd, "\n", 1);
        free(input);
    }
    close(fd);
    return (0);
}

int start_here_doc(const char *eof_word, char **filename)
{
    int fd;
    int result;
    void (*old_handler)(int);

    if (!filename)
        return (1);
    if (*filename)
    {
        delete_here_doc(*filename);
        free(*filename);
        *filename = NULL;
    }
    g_signal_received = 0;
    rl_done = 0;
    old_handler = signal(SIGINT, handle_sigint_heredoc);
    if (create_file(filename, &fd))
    {
        signal(SIGINT, old_handler);
        return (1);
    }
    result = write_here_doc(eof_word, filename, fd);
    g_signal_received = 0;
    rl_done = 0;
    signal(SIGINT, old_handler);
    return (result);
}
