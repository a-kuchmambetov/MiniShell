# Makefile
NAME    = minishell
LIB     = libminishell.a

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
LDLIBS  = -lreadline
LIBS    = libminishell.a

LIBFTDIR = libft
LIBFT    = $(LIBFTDIR)/libft.a

SRC = 	src/executor/executor_command_info.c \
		src/executor/executor_io.c \
		src/executor/executor_child.c \
		src/executor/executor_parent.c \
		src/executor/executor_path.c \
		src/executor/executor_wait.c \
		src/executor/executor_ctx.c \
		src/executor/executor_iteration.c \
		src/executor/executor_run.c \
		src/my_free.c \
        src/free_utils.c \
        src/ft_print_err.c \
        src/init_shell_data.c \
		src/parse_envp.c \
		src/update_last_exit_status.c

SRC +=	src/built_in_commands/built_in_commands.c \
		src/built_in_commands/echo_command.c \
		src/built_in_commands/export_command.c \
		src/built_in_commands/export_command_utils.c \
		src/built_in_commands/cd_command.c \
		src/built_in_commands/cd_command_utils.c \
		src/built_in_commands/pwd_command.c \
		src/built_in_commands/env_command_utils.c \
		src/built_in_commands/env_utils.c \
		src/built_in_commands/utils.c \
		src/built_in_commands/env_command.c \
		src/built_in_commands/unset_command.c \
		src/built_in_commands/exit_command.c 
# Parser SRC files
SRC += 	src/new_parser/split_input/split_input.c \
		src/new_parser/split_input/split_input_utils.c
# - Token related files
# -- Create token list
SRC += 	src/new_parser/token/create_token_list/create_token_list.c \
		src/new_parser/token/create_token_list/set_tkn.c \
		src/new_parser/token/create_token_list/free_token_list.c \
		src/new_parser/token/create_token_list/free_token_node.c
# -- Expande tokens
SRC += 	src/new_parser/token/expand_tokens/expand_tokens.c \
		src/new_parser/token/expand_tokens/expand_tokens_utils.c \
		src/new_parser/token/expand_tokens/is_ambig.c \
		src/new_parser/token/expand_tokens/squash_spaces.c
# -- Split expansion
SRC += 	src/new_parser/token/split_expansion/split_expansion.c
# -- Merge tokens
SRC += 	src/new_parser/token/merge_tokens/merge_tokens.c \
		src/new_parser/token/merge_tokens/trim_quotes.c
# 		
SRC +=	src/new_parser/create_cmd_list/create_cmd_list.c \
		src/new_parser/create_cmd_list/create_cmd_list_utils.c \
		src/new_parser/create_cmd_list/check_file.c \
		src/new_parser/create_cmd_list/cmd_node_utils.c
# 
SRC +=	src/new_parser/start_here_doc/start_here_doc_utils.c \
		src/new_parser/start_here_doc/file_utils.c \
		src/new_parser/start_here_doc/start_here_doc.c 
# 
SRC +=	src/new_parser/parser.c

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

re: fclean all clean

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

compileTest7: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_7.c $(LIBS) $(LDLIBS) -o $(NAME)_test_7

compileTest6New: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_6_new.c $(LIBS) $(LDLIBS) -o $(NAME)_test_6_new

compileTestExit: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_exit.c $(LIBS) $(LDLIBS) -o $(NAME)_test_exit

compileTestPipelineParser: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_pipeline_parser.c $(LIBS) $(LDLIBS) -o $(NAME)_test_pipeline_parser

compileTestBuiltins: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/test_main_builtins.c tests/process_input_for_tests.c $(LIBS) $(LDLIBS) -o $(NAME)_test_builtins

compileTestFull: $(LIBFT) $(LIB)
	$(CC) $(CFLAGS) tests/minishell_full_test.c $(LIBS) $(LDLIBS) -lreadline -o $(NAME)_test_full

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --show-leak-kinds=definite ./minishell

.PHONY: all clean fclean allClean re \
	compileTest1 compileTest2 compileTest3 compileTest4 compileTest5 compileTest6 compileTest6New \
	compileTestExit compileTestPipelineParser compileTestBuiltins compileTestFull
