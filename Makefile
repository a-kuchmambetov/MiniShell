# Makefile
NAME    = minishell

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
LIBS    = -lreadline

LIBFTDIR = libft
LIBFT    = $(LIBFTDIR)/libft.a

SRC = src/exec_cmd.c \
		src/free_utils.c \
		src/ft_print_err.c \
		src/split_input_str.c \
		src/print_prompt_header.c \
		src/set_shell_data.c \
		src/split_input_str_utils/split_input_str_utils.c \

OBJS     = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) main.c $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

clean:
	rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: 
	rm -rf $(OBJS)
	rm -f $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

mlxDel:
	rm -rf mlx

allClean: fclean 
	rm -rf test_minishell_1 test_minishell_2 \
	tests/test_main_1 tests/test_main_2

re: fclean all 

compileTest1: $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) tests/test_main_1.c $(OBJS) $(LIBFT) $(LIBS) -o test_minishell_1

compileTest2: $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) tests/test_main_2.c $(OBJS) $(LIBFT) $(LIBS) -o test_minishell_2

.PHONY: all clean fclean mlxDel allClean re
