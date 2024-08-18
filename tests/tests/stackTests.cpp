/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stackTests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:21:59 by akernot           #+#    #+#             */
/*   Updated: 2024/08/08 18:28:58 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stackTests.hpp"
#include "stack.h"

void stackCreateDestroy::test(int logFD) const
{
	t_stack *stack = NULL;
	stack = stack_ctor(10);
	fassert(logFD, stack != NULL);
	fassert(logFD, stack->data != NULL);
	fassert(logFD, stack->data->array != NULL);
	fassert(logFD, stack->data->size == 0);
	fassert(logFD, stack->data->capacity == 10);
}

void stackNullPush::test(int logFD) const
{

}

void stackBasicPush::test(int logFD) const
{

}

void stackMultiplePush::test(int logFD) const
{

}

void stackNullPop::test(int logFD) const
{

}

void stackPop::test(int logFD) const
{

}

void stackPopTooMany::test(int logFD) const
{

}

void stackFrontNull::test(int logFD) const
{

}

void stackFrontPush::test(int logFD) const
{

}

void stackFrontPop::test(int logFD) const
{

}

stackTestList::stackTestList()
	: fileTests("stackTests", {
		new stackCreateDestroy(),
		new stackNullPush(),
		new stackBasicPush(),
		new stackMultiplePush(),
		new stackNullPop(),
		new stackPop(),
		new stackPopTooMany(),
		new stackFrontNull(),
		new stackFrontPush(),
		new stackFrontPop()
	})
	{	}
