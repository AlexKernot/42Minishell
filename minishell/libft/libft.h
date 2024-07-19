/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 21:24:21 by akernot           #+#    #+#             */
/*   Updated: 2024/07/19 16:30:58 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>

typedef enum e_bool
{
	error = -1,
	false = 0,
	true = 1
}	t_bool;

int			ft_isalpha(char c);
int			ft_isdigit(char c);
int			ft_isalnum(char c);
int			ft_isascii(char c);
int			ft_isprint(char c);
size_t		ft_strlen(const char *s);
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *haystack, const char *needle, size_t n);

/* String convert */

char		*ft_itoa(int n);
int			ft_atoi(const char *nptr);
char		*ft_ltoa(long num);

/* Print */

t_bool		ft_putchar_fd(const char c, const int fd);
t_bool		ft_putstr_fd(const char *s, const int fd);
t_bool		ft_putendl_fd(const char *s, const int fd);
t_bool		ft_putnbr_fd(const int n, const int fd);

t_bool		ft_putstr(const char *string);
t_bool		ft_putchar(const char c);
t_bool		ft_putendl(const char *s);
t_bool		ft_putnbr(const int n);

int			ft_printf(const char *format, ...);

/* Vector */

typedef struct s_vector
{
	void	**array;
	int		capacity;
	int		size;
}	t_vector;

/* Creates a vector with a pointer to an array of ints of size capacity       */
t_vector	*vector_ctor(int capacity);

/* Frees the entire array and node. Setting the pointer to NULL.              */
void		vector_dtor(t_vector **vector);

/* Adds an integer to the end of the int array, resizing if nessecary.        */
/* This function returns the new size of the array. */
void		vector_push_back(t_vector *vector, void *data);

/* Removes the last index from the array.                                     */
void		vector_pop_back(t_vector *vector);

/* Resizes the vector to size, copying the data from the old array into the   */
/* new one. This will only copy size bytes of data into the new array,        */
/* deleting all extra bytes.                                                  */
void		vector_resize(t_vector *vector, int size);

/* Inserts data at index. This function will overwrite data.                  */
void		vector_set(t_vector *vector, int index, void *data);

/* Returns a pointer to the t_packet at index, or NULL if failed.             */
void		*vector_get(t_vector *vector, int index);

/* Returns a pointer to the value at the end of the vector.                   */
void		*vector_end(t_vector *vector);

/* Returns a pointer to the value at the front of the vector. */
void		*vector_front(t_vector *vector);

/* Stack */
/* A last in first out data structure that creates a vector of            */
/* integer pointers. the output of stack_pop() needs to be freed.         */
/* On destruction, stack_dtor() will clean up all data held by the stack. */
typedef struct s_stack
{
	t_vector	*data;
}	t_stack;

/* Creates a new stack on the heap and returns a pointer to it. */
t_stack		*stack_ctor(int stack_size);

/* Frees the stack, and and all contained data. This function invalidates */
/* all pointers to this stack and inside data. */
/* It also sets the passed in pointer to stack to NULL. */
void		stack_dtor(t_stack **this_stack);

/* Adds the given void * to the top of the stack */
void		stack_push(t_stack *this_stack, void *data);

/* Removes the number on the top of the stack and returns it. */
void		*stack_pop(t_stack *this_stack);

/* Returns the value at the top of the stack. */
/* This function may return NULL. */
void		*stack_front(t_stack *this_stack);

void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, const char *set);
char		**ft_split(const char *s, char c);

char		*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
char		*get_next_line(int fd);
#endif
