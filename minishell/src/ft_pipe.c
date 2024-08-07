/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 20:16:15 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 23:17:44 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define WRITE_PIPE 1
#define READ_PIPE 0

int	ft_pipe(int fds[2])
{
	char	*file_name;
	int		read_fd;
	int		write_fd;

	file_name = create_tmp();
	write_fd = open(file_name, O_WRONLY | O_CREAT, 0666);
	if (write_fd == -1)
	{
		perror("minishell: pipefd");
		return (-1);
	}
	read_fd = open(file_name, O_RDONLY);
	if (read_fd == -1)
	{
		perror("minishell: pipefd");
		return (-1);
	}
	fds[WRITE_PIPE] = write_fd;
	fds[READ_PIPE] = read_fd;
	unlink(file_name);
	free(file_name);
	return (1);
}
