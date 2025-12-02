#include "../main.h"

void my_exit(int code, t_shell_data *data)
{
    free_shell_data(data);
    exit(code);
}