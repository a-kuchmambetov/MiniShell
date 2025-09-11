#include "../main.h"

static char *build_prompt_header(void)
{
    char	*cwd;
    char	*hdr;

    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (NULL);
    hdr = ft_calloc(1, ft_strlen(COLOR_CYAN) + ft_strlen(cwd)
        + ft_strlen(COLOR_RESET) + 1);
    if (!hdr)
        return (free((char *)cwd), (char *)NULL);
    ft_strlcpy(hdr, COLOR_CYAN, ft_strlen(COLOR_CYAN) + 1);
    ft_strlcat(hdr, cwd, ft_strlen(hdr) + ft_strlen(cwd) + 1);
    ft_strlcat(hdr, COLOR_RESET, ft_strlen(hdr) + ft_strlen(COLOR_RESET) + 1);
    free(cwd);
    return (hdr);
}

void print_prompt_header(void)
{
    char *hdr;

    hdr = build_prompt_header();
    if (!hdr)
        return ;
    ft_printf("\n%s\n", hdr);
    free(hdr);
}