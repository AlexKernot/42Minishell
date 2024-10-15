# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/04 17:22:57 by akernot           #+#    #+#              #
#    Updated: 2024/10/15 18:54:53 by alexkernot       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = g++
CXXFLAGS = -Wall -DDEBUG -Wextra -fno-builtin -g -I../includes -I./tests/ -I./tests/includes -I../libft -std=c++17
LDFLAGS = -lreadline

OUTPUTDIR = ./bin

TESTS = runTests.cpp fileTests.cpp individualTest.cpp addSymbolAndWordTest.cpp \
tester.cpp copyStringTest.cpp extractStringTests.cpp getTypeTests.cpp \
testerTest.cpp parseTests.cpp tokenListTests.cpp syntaxTreeTests.cpp \
shuntingYardTests.cpp fakeMalloc.cpp expandEnvVariableTest.cpp splitTest.cpp \
IntegrationTests.cpp ComplexIntegrationTest.cpp

VPATH = ./tests/ ./tests/tests

OBJ = $(TESTS:%.cpp=$(OUTPUTDIR)/%.o)

$(OUTPUTDIR)/%.o: %.cpp
	mkdir -p $(OUTPUTDIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: unit

lib:
	make lib -C ../
	mv ../minishell.a ./minishell.a

binary:
	make -C ../
	mv ../minishell ./integrationTests/minishell

unit: lib binary $(OBJ)
	$(CXX) $(LDFLAGS) ./minishell.a ../bin/libft.a $(OBJ)

clean:
	-rm -f $(OBJ) minishell.a ./integrationTests/minishell
	-rm -d $(OUTPUTDIR)

fclean: clean
	-rm -f a.out

re: fclean all
	
