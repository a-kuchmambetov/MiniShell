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
		src/my_free.c \
        src/free_utils.c \
        src/ft_print_err.c \
        src/print_prompt_header.c \
		src/update_last_cmd_code.c \
        src/set_shell_data.c \
		src/split_input_str_utils/split_input_str_utils.c \
        src/split_input_str.c \
		src/process_expansion_utils/process_expansion_utils.c \
		src/process_expansion.c \
		src/start_here_doc_utils/file_utils.c \
		src/start_here_doc_utils/start_here_doc_utils.c \
		src/start_here_doc.c \
		src/parse_input_utils/trim_quote.c \
		src/parse_input_utils/expand_input_arr.c \
		src/parse_input_utils/polish_input_arr_utils_small.c \
		src/parse_input_utils/polish_input_arr_utils_arr.c \
		src/parse_input_utils/polish_input_arr.c \
		src/parse_input.c \
		src/create_cmd_list.c \
		src/create_cmd_list_utils/create_cmd_list_utils.c \
		src/create_cmd_list_utils/check_file.c \
		src/create_cmd_list_utils/cmd_node_utils.c \
		src/built_in_commands/built_in_commands.c \
		src/built_in_commands/echo_command.c \
		src/built_in_commands/echo_command_utils.c \
		src/built_in_commands/export_command.c \
		src/built_in_commands/cd_command.c \
		src/built_in_commands/cd_command_utils.c \
		src/built_in_commands/pwd_command.c \
		src/built_in_commands/env_command_utils.c \
		src/built_in_commands/utils.c \
		src/built_in_commands/env_command.c \
		src/built_in_commands/unset_command.c \
		src/built_in_commands/exit_command.c 

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

# expansion tests
compileTest6: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_6.c $(LIBS) $(LDLIBS) -o $(NAME)_test_6

compileTestExit: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_exit.c $(LIBS) $(LDLIBS) -o $(NAME)_test_exit

compileTestPipelineParser: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_pipeline_parser.c $(LIBS) $(LDLIBS) -o $(NAME)_test_pipeline_parser

compileTestBuiltins: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_builtins.c $(LIBS) $(LDLIBS) -o $(NAME)_test_builtins

compileTestFull: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/minishell_full_test.c $(LIBS) $(LDLIBS) -lreadline -o $(NAME)_test_full

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	--trace-children=yes ./minishell

.PHONY: all clean fclean allClean re \
	compileTest1 compileTest2 compileTest3 compileTest4 compileTest5 compileTest6 \
	 compileTestExit compileTestPipelineParser compileTestBuiltins compileTestFull
