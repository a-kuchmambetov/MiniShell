#include "../main.h"

void my_free(void *ptr)
{
    if (ptr)
        free(ptr);
    ptr = NULL;
}