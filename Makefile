# Makefile
NAME    = minishell

CC      = cc
CFLAGS  = -Wall -Wextra -Werror
LIBS    = -lreadline

LIBFTDIR = libft
LIBFT    = $(LIBFTDIR)/libft.a

SRC      = main.c \
		src/exec_cmd.c \
		src/free_utils.c \
		src/ft_print_err.c \
		src/parse_input.c \
		src/process_fd.c \
		src/set_envp_from_env.c 
OBJS     = $(SRC:.c=.o)

# Source files excluding main.c for tests
TEST_SRC_1 = src/exec_cmd.c \
		src/free_utils.c \
		src/ft_print_err.c \
		src/parse_input.c \
		src/process_fd.c \
		src/set_envp_from_env.c 
TEST_OBJS_1 = $(TEST_SRC_1:.c=.o)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

clean:
	rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: 
	rm -rf $(OBJS)
	rm -f $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

mlxDel:
	rm -rf mlx

allClean: fclean mlxDel

re: fclean all

compileTest1: all
	$(CC) tests/test_main_1.c $(TEST_OBJS_1) $(LIBFT) $(LIBS) -o test_minishell_1

.PHONY: all clean fclean mlxDel allClean re
