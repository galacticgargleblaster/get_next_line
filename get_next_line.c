/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/01 10:59:50 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"
#include "libft/includes/libft.h"

static gnl_context_t	*get_existing_context_for_fd(const int fd, t_list *elt)
{
	while (elt && elt->content)
	{
		if (((gnl_context_t*)(elt->content))->fd == fd)
			return (elt->content);
		elt = elt->next;
	}
	return (NULL);
}

static gnl_context_t	*__gnl_context_new__(const int fd)
{
	gnl_context_t	*context;

	context = malloc(sizeof(gnl_context_t));
	if (context == NULL)
		return (NULL);
	ft_bzero(context->buf, BUFF_SIZE);
	context->line = NULL;
	context->fd = fd;
	context->read_return_value = BUFF_SIZE;
	return (context);
}

static void				__gnl_context_del__(gnl_context_t *context)
{
	free(context->buf);
	free(context);
}


#define CONTAINS_NEWLINE(STR) (ft_strchr(STR, '\n') != NULL)
#define NOT(X) (!(X))

static int				get_next_line_in_context(gnl_context_t *c, char **line)
{
	char			*tmp;

	while ((c->read_return_value = read(c->fd, c->buf, BUFF_SIZE)) != 0)
	{
		if (c->read_return_value < 0)
			return (GET_NEXT_LINE_READ_ERROR);
		c->line_size += (CONTAINS_NEWLINE(c->buf) ? 
						(size_t)(ft_strchr(c->buf, '\n') - c->buf) : 
						c->read_return_value);
		if ((tmp = ft_strnew(c->line_size)) == NULL)
			return (GET_NEXT_LINE_READ_ERROR);
		if (c->line && 
				(ft_strcpy(tmp, c->line) == NULL ||
				ft_strncat(tmp, c->buf, c->line_size) == NULL))
			return (GET_NEXT_LINE_READ_ERROR);
		else
			free(c->line);
		c->line = tmp;
		*line = c->line;
		if (CONTAINS_NEWLINE(c->buf))
			return (GET_NEXT_LINE_READ_SUCCESS);
	}
	return (GET_NEXT_LINE_READ_COMPLETE);
}

/*
**	Returns when '\n' is encountered, when read() returns 0, or
**  when an error occurs.
*/

int						get_next_line(const int fd, char **line)
{
	static t_list	*contexts;
	gnl_context_t	*c;
	int				return_value;

	c = get_existing_context_for_fd(fd, contexts);
	if (c == NULL)
	{
		c = __gnl_context_new__(fd);
		if (c == NULL)
			return (GET_NEXT_LINE_READ_ERROR);
		contexts = ft_lstpush(contexts, c);
	}	
	return_value = get_next_line_in_context(c, line);
	if (return_value == GET_NEXT_LINE_READ_ERROR ||
		return_value == GET_NEXT_LINE_READ_COMPLETE)
	{
		c = ft_lstremove(&contexts, c);
		__gnl_context_del__(c);
	}
	return (return_value);
}
