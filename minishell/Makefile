NAME=minishell

CC = clang
CFLAGS = -MD -Wall -Wextra -Wpedantic -g -std=c11 -I./libft/ -I./includes/
LDFLAGS = -lreadline

FILES = run.c execute.c vector_subset.c process.c hash_func.c \
		process_symbols.c subshell.c ft_transmit.c start_interactive.c \
		get_shell_input.c env_vars.c main.c search_env_vars.c \
		redirect.c cleanup.c preprocess.c ft_pipe.c \
		builtin_echo.c builtin_env.c builtin_exit.c builtin_export.c \
		builtin_pwd.c builtin_unset.c builtin_cd.c \
		tokenizer.c token_list.c syntax_tree.c shunting_yard.c \
		ft_stack.c evaluator.c expand.c

OUTPUTDIR = ./bin

VPATH = ./src/ ./src/builtins ./src/tokenizer ./src/parser ./src/stack ./src/evaluator/

LIBFT = $(OUTPUTDIR)/libft.a

OBJ=$(FILES:%.c=$(OUTPUTDIR)/%.o)
DEPS=$(OBJ:.o=.d)

$(OUTPUTDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(LIBFT):
	make all -C ./libft/
	mkdir -p $(OUTPUTDIR)
	mv libft/libft.a $(LIBFT)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)

lib: $(LIBFT) $(OBJ)
	ar -rcs minishell.a $(OBJ)

clean:
	-rm -f $(OBJ) $(DEPS) $(LIBFT)
	-rm -d $(OUTPUTDIR)
	-make clean -C./libft/

fclean: clean
	rm -f $(NAME)

re: fclean all

syntax:
	$(CC) -fsyntax-only -I./libft/ -I./includes/ -Weverything -Wno-newline -Wno-padded src/**.c

.PHONEY: fclean clean all re

-include $(DEPS)
