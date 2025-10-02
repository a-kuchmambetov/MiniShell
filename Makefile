# Makefile
NAME    = minishell
LIB     = libminishell.a

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
LIBS    = -lreadline libminishell.a

LIBFTDIR = libft
LIBFT    = $(LIBFTDIR)/libft.a

SRC = src/exec_cmd.c \
        src/free_utils.c \
        src/ft_print_err.c \
        src/split_input_str.c \
        src/print_prompt_header.c \
        src/set_shell_data.c \
        src/split_input_str_utils/split_input_str_utils.c \
		src/create_cmd_list.c 

OBJS     = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(LIB): $(OBJS)
	cp $(LIBFT) $(LIB)
	ar rcs $(LIB) $(OBJS)

$(NAME): $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) $(LIBS) main.c -o $(NAME)

clean:
	rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: 
	rm -rf $(OBJS)
	rm -f $(NAME) $(LIB)
	$(MAKE) -C $(LIBFTDIR) fclean

mlxDel:
	rm -rf mlx

allClean: fclean 
	rm -rf $(NAME)_test_* \
	tests/test_main_1 tests/test_main_2 tests/test_main_3

re: fclean all 

compileTest1: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_1.c $(LIBS) -o $(NAME)_test_1

compileTest2: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_2.c $(LIBS) -o $(NAME)_test_2

compileTest3: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_3.c $(LIBS) -o $(NAME)_test_3

.PHONY: all clean fclean mlxDel allClean re compileTest1 compileTest2