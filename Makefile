# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/04 17:22:57 by akernot           #+#    #+#              #
#    Updated: 2024/07/09 17:47:15 by akernot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CXXFLAGS = -Wall -Wextra -fno-builtin -g -I./minishell/includes -I./minishell/libft -std=c++20
LDFLAGS = -lreadline

OUTPUTDIR = ./bin

TESTS = runTests.cpp copyStringTest.cpp extractStringTests.cpp getTypeTests.cpp \
fakeMalloc.cpp fakeMemoryTest.cpp tokenListTests.cpp addSymbolAndWordTest.cpp
VPATH = ./tests/
OBJ = $(TESTS:%.cpp=$(OUTPUTDIR)/%.o)

$(OUTPUTDIR)/%.o: %.cpp
	mkdir -p $(OUTPUTDIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: test

test: $(OBJ)
	make lib -C ./minishell/
	mv ./minishell/minishell.a ./minishell.a
	$(CXX) $(LDFLAGS) ./minishell.a ./minishell/bin/libft.a $(OBJ)

clean:
	-rm -f $(OBJ) minishell.a
	-rm -d $(OUTPUTDIR)

fclean: clean
	-rm -f a.out

re: fclean all
	