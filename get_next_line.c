/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/01 20:31:27 by nkirkby          ###   ########.fr       */
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
	context->line_size = 0;
	context->read_return_value = BUFF_SIZE;
	return (context);
}

/*
**	checks for '\n', reallocates and concatenates buffer contents
**	onto existing line.
*/

static int				debuffer_line(gnl_context_t *c)
{
	char				*tmp;

	if ((tmp = ft_strnew(c->line_size)) == NULL)
		return (GET_NEXT_LINE_READ_ERROR);
	if (c->line)
	{
		if (ft_strcpy(tmp, c->line) == NULL)
			return (GET_NEXT_LINE_READ_ERROR);
		free(c->line);
	}
	if (ft_strncat(tmp, c->buf, c->line_size - ft_strlen(tmp)) == NULL)
		return (GET_NEXT_LINE_READ_ERROR);
	c->line = tmp;
	return (0);
}

#define CONTAINS_NEWLINE(STR) (ft_strchr(STR, '\n') != NULL)

static int				get_next_line_in_context(gnl_context_t *c, char **line)
{
	while ((c->read_return_value = read(c->fd, c->buf, BUFF_SIZE)) != 0)
	{
		if (c->read_return_value < 0)
			return (GET_NEXT_LINE_READ_ERROR);
		c->line_size += (CONTAINS_NEWLINE(c->buf) ? 
						(size_t)(ft_strchr(c->buf, '\n') - c->buf) : 
						c->read_return_value);
		if (debuffer_line(c) == GET_NEXT_LINE_READ_ERROR)
			return (GET_NEXT_LINE_READ_ERROR);
		if (CONTAINS_NEWLINE(c->buf))
		{
			*line = c->line;
			return (GET_NEXT_LINE_READ_SUCCESS);
		}
	}
	return (GET_NEXT_LINE_READ_COMPLETE);
}

/*
**	Returns with 1 when '\n' is encountered, with 0 when read() returns 0, or
**  with -1 when an error occurs.
*/

int						get_next_line(const int fd, char **line)
{
	static t_list	*contexts;
	gnl_context_t	*c;
	int				return_value;

	*line = NULL;
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
		free(c);
	}
	return (return_value);
}
