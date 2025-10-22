# Makefile
NAME    = minishell
LIB     = libminishell.a

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
LDLIBS  = -lreadline
LIBS    = libminishell.a

LIBFTDIR = libft
LIBFT    = $(LIBFTDIR)/libft.a

SRC = src/exec_cmd.c \
        src/free_utils.c \
        src/ft_print_err.c \
        src/split_input_str.c \
        src/print_prompt_header.c \
        src/set_shell_data.c \
        src/split_input_str_utils/split_input_str_utils.c \
		src/create_cmd_list.c \
		src/create_cmd_list_utils/create_cmd_list_utils.c \
		src/create_cmd_list_utils/check_file.c \
		src/create_cmd_list_utils/cmd_node_utils.c \
		src/built_in_comands/built_in_comands.c \
		src/built_in_comands/echo_comand.c \
		src/built_in_comands/export.c \
		src/built_in_comands/cd_comand.c \
		src/built_in_comands/pwd_comand.c \
		src/built_in_comands/env_utils.c \
		src/built_in_comands/utils.c \
		src/built_in_comands/env_comand.c \
		src/built_in_comands/unset_comand.c \
		src/built_in_comands/exit_comand.c \
		src/start_here_doc_utils/file_utils.c \
		src/start_here_doc_utils/start_here_doc_utils.c \
		src/start_here_doc.c \
		src/process_expansion_utils/process_expansion_utils.c \
		src/process_expansion.c

OBJS     = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(LIB): $(OBJS) 
	cp $(LIBFT) $(LIB)
	ar rcs $(LIB) $(OBJS)

$(NAME): $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) main.c $(LIBS) $(LDLIBS) -o $(NAME)

clean:
	rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean:
	rm -rf $(OBJS)
	rm -f $(NAME) $(LIB)
	$(MAKE) -C $(LIBFTDIR) fclean

allClean: fclean
	find -maxdepth 1 -type f \( -name '$(NAME)_test_*' -o -name 'here_doc_*' \) ! -name '*.c' ! -name '*.h' -delete
	find tests -maxdepth 1 -type f -name 'test_main_*' ! -name '*.c' -delete

re: fclean all

compileTest1: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_1.c $(LIBS) $(LDLIBS) -o $(NAME)_test_1

# split_input_str tests
compileTest2: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_2.c $(LIBS) $(LDLIBS) -o $(NAME)_test_2

# exec_cmd tests
compileTest3: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_3.c $(LIBS) $(LDLIBS) -o $(NAME)_test_3

# set_here_doc tests
compileTest4: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_4.c $(LIBS) $(LDLIBS) -o $(NAME)_test_4

# expansion tests
compileTest5: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_5.c $(LIBS) $(LDLIBS) -o $(NAME)_test_5

compileTestExit: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_exit.c $(LIBS) $(LDLIBS) -o $(NAME)_test_exit

compileTestPipelineParser: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_pipeline_parser.c $(LIBS) $(LDLIBS) -o $(NAME)_test_pipeline_parser

compileTestBuiltins: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_builtins.c $(LIBS) $(LDLIBS) -o $(NAME)_test_builtins

compileTestFull: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/minishell_full_test.c $(LIBS) $(LDLIBS) -lreadline -o $(NAME)_test_full


.PHONY: all clean fclean allClean re compileTest1 compileTest2 compileTest3 compileTest4 compileTestExit compileTestPipelineParser compileTestBuiltins compileTestFull
