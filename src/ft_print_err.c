#include "../main.h"

int ft_print_char(char s)
{
    write(2, &s, 1);
    return (1);
}

int ft_print_str(char *s)
{
    int len;

    if (!s)
        s = "(null)";
    len = ft_strlen(s);
    write(2, s, len);
    return (len);
}

int ft_print_err(const char *format, ...)
{
    int len;
    int i;

    va_list args;
    va_start(args, format);
    len = 0;
    i = 0;

    // ft_print_str(COLOR_RED "\nminishell: ");
    while (format[i])
    {
        if (format[i] == '%')
        {
            if (format[++i] == 's')
                len += ft_print_str(va_arg(args, char *));
            else if (format[i] == 'd')
                len += ft_putnbr_base(va_arg(args, int), "0123456789");
        }
        else
            len += ft_print_char(format[i]);
        i++;
    }
    // ft_print_str(COLOR_RESET);
    va_end(args);
    return (len);
}
