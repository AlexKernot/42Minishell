/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:54:56 by akernot           #+#    #+#             */
/*   Updated: 2024/01/25 19:41:11 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_VARIABLES_H
# define ENVIRONMENT_VARIABLES_H

typedef struct s_env_vars
{
	char				*name;
	char				*val;
	struct s_env_vars	*next;
}	t_env_vars;

char		*expand_env_vars(char *tmp, int last_return);
t_env_vars	**get_env_vars(void);
int			env_len(void);
void		init_env_vars(char *const envp[]);
char		**compress_env_vars(void);
t_env_vars	*find_env_var(char *name);
void		clear_env_vars(void);
char		*ft_getenv(char	*const name);
int			find_equal_sign(char *const string);

#endif
