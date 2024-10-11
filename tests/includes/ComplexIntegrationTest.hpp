/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ComplexIntegrationTest.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:58:00 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 17:33:21 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLEXINTEGRATIONTEST_HPP
# define COMPLEXINTEGRATIONTEST_HPP

# include "fileTests.hpp"

class ComplexIntegrationTest : public fileTests
{
public:
	ComplexIntegrationTest();
};

class HeredocIntegrationTest : public fileTests
{
public:
	HeredocIntegrationTest();
};

#endif // COMPLEXINTEGRATIONTEST_HPP