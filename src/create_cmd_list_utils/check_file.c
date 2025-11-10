#include "create_cmd_list_utils.h"

// Check with chatGPT for edge cases regarding filenames
int check_file_open(const char *filename)
{
    const int fd = open(filename, O_RDONLY);

    // if ((*filename < 'A' || *filename > 'z') && (*filename < '0' || *filename > '9'))
    if (*filename == '\0' || *filename == '/')
    {
        ft_print_err("syntax error near unexpected token `%s'\n", filename);
        return (close(fd), 1);
    }
    if (fd == -1)
    {
        ft_print_err("%s: No such file or directory\n", filename);
        return (close(fd), 1);
    }
    close(fd);
    return (0);
}

int check_file_create(const char *filename, const int redir_type)
{
    int fd;

    if ((*filename < 'A' || *filename > 'z') && (*filename < '0' || *filename > '9'))
    {
        ft_print_err("syntax error near unexpected token `%s'\n", filename);
        return (1);
    }
    if (redir_type == REDIR_APPEND)
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_print_err("%s: No such file or directory\n", filename);
        return (1);
    }
    close(fd);
    return (0);
}