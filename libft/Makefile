# Makefile
# Source files and object files
SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
ft_printf.c ft_printf_utils.c
OBJ = $(SRC:.c=.o)

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Library and executable names
NAME = libft.a

# Default target: build the library and link it into the executable
all: $(NAME)

# Build the static library from object files (excluding main.o)
$(NAME): $(OBJ)
	ar rcs ${NAME} ${OBJ}

# Clean up
clean:
	rm -f ${OBJ}

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

# Testing commands
test1: all clean
	curl -O https://raw.githubusercontent.com/OneEyedWilly293/Libft/master/function_test.c; cc function_test.c -Wall -Wextra -Werror -g -lbsd libft.a; ./a.out > test1_res.tt; rm -f function_test.c ./a.out; fclean

test2:
	git clone https://github.com/Tripouille/libftTester.git; cd libftTester; echo "Testing, Please wait.";make m;

test2c:
	rm -rf libftTester ${NAME} ${OBJ};

.PHONY: all clean fclean re test1 test2 test2c
