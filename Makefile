# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/04 17:22:57 by akernot           #+#    #+#              #
#    Updated: 2024/08/16 18:42:26 by akernot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CXXFLAGS = -Wall -Wextra -DDEBUG -fno-builtin -g -I./minishell/includes -I./tests/ -I./tests/includes -I./minishell/libft -std=c++20
LDFLAGS = -lreadline

OUTPUTDIR = ./bin

TESTS = runTests.cpp fileTests.cpp individualTest.cpp addSymbolAndWordTest.cpp \
tester.cpp copyStringTest.cpp extractStringTests.cpp getTypeTests.cpp \
testerTest.cpp parseTests.cpp tokenListTests.cpp syntaxTreeTests.cpp \
shuntingYardTests.cpp fakeMalloc.cpp expandEnvVariableTest.cpp


VPATH = ./tests/ ./tests/tests

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
	