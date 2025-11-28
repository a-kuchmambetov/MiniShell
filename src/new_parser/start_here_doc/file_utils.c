#include "start_here_doc.h"

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
        ft_print_err("error: unable to create here-document temp file\n");
        free(*filename);
        *filename = NULL;
        return (1);
    }
    return (0);
}
