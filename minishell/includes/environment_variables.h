/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:54:56 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:59:00 by akernot          ###   ########.fr       */
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
t_env_vars	*make_env_var(t_env_vars *current, char *name, char *value);
char		**compress_env_vars(void);
t_env_vars	*find_env_var(char *name);
void		clear_env_vars(void);
char		*ft_getenv(char	*const name);
t_env_vars	*env_push_front(void);
int			find_equal_sign(char *const string);

#endif
