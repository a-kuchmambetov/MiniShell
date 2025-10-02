#ifndef TOKENIZER_UTILS_H
# define TOKENIZER_UTILS_H

# include "../../libft/libft.h"

int	ft_is_space(char c);
int	ft_op_len(const char *s);
int	ft_word_len(const char *s);
void	ft_skip_ws(const char *s, int *i);

int	ft_handle_pipe_len(const char *s, int *i, int *len);
int	ft_add_word_len(const char *s, int *i, int *len);
int	ft_len_normal(const char *s, int i, int *consumed);
int	ft_len_redir(const char *s, int i, int *consumed);
void	ft_advance_and_count(const char *s, int *i, int *c);

int	ft_append_pipe(const char *s, char *dst, int *i, int *pos);
int	ft_copy_word(const char *s, char *dst, int *i, int *pos);
void	ft_fill_normal_token(const char *s, char *tok, int *i, int stop, int *pos);

#endif
