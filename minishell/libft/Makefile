NAME=libft.a

CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -Wno-newline-eof -Werror -I./ -I./print/printf/includes

OUTPUTFOLDER=binaries/

FILES=	ft_atoi.c ft_itoa.c ft_ltoa.c get_next_line.c get_next_line_utils.c \
		ft_vector_utils.c ft_vector.c ft_lstadd_back_bonus.c \
		ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c \
		ft_lstiter_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c \
		ft_lstsize_bonus.c ft_bzero.c ft_calloc.c ft_memchr.c ft_memcmp.c \
		ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putchar.c \
		ft_putendl_fd.c ft_putendl.c ft_putnbr.c ft_putnbr_fd.c ft_putstr.c \
		ft_putstr_fd.c ft_putchar_fd.c ft_stack.c ft_isalnum.c ft_isalpha.c \
		ft_isascii.c ft_isdigit.c ft_isprint.c ft_split.c ft_strchr.c \
		ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c \
		ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
		ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
		get_next_line_bonus.c get_next_line_utils_bonus.c ft_hexadecimal.c \
		ft_integer.c ft_parse_bonus.c ft_pointer.c ft_print_utils.c \
		ft_printf_bonus.c ft_string.c ft_ultoa.c

BINARIES=$(FILES:%.c=$(OUTPUTFOLDER)%.o)

VPATH=%.c convert/ gnl/ list/ memory/ print/ print/printf/ \
stack/ string/ vector/

$(OUTPUTFOLDER)%.o : %.c
	@mkdir -p $(OUTPUTFOLDER)
	@$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(BINARIES)
	@ar -rs $(NAME) $(BINARIES)
	@echo "Built libft"

clean:
	-rm -f $(BINARIES)
	-rm -fd $(OUTPUTFOLDER)

fclean: clean
	-rm -f $(NAME)

re: fclean all

.PHONY: clean all fclean re