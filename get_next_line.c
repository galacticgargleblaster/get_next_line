/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/02 16:10:24 by nkirkby          ###   ########.fr       */
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
	context->line_start = context->buf;
	context->debuffer_state = DEBUFFER_STATE_INIT;
	return (context);
}

/*
**	concatenates buffer contents onto existing context's line,
**  reallocating memory for the line if necesssary.
*/

#define EFFECTIVE_BUFF_SIZE(c) (MIN(BUFF_SIZE, c->read_return_value))
#define BYTES_REMAINING(c) (EFFECTIVE_BUFF_SIZE(c) - (c->line_start - c->buf))

static int				debuffer_line(gnl_context_t *c)
{
	char				*tmp;
	size_t				next_segment_length;
	char				*next_nl;

	if ((next_nl = ft_memchr(c->line_start, '\n', BYTES_REMAINING(c))) == NULL)
		next_segment_length = BYTES_REMAINING(c);
	else
		next_segment_length = next_nl - c->line_start;
	c->line_size += next_segment_length;
	if ((tmp = ft_strnew(c->line_size)) == NULL)
		return (DEBUFFER_STATE_ERROR);
	if (c->line)
	{
		if (ft_strcpy(tmp, c->line) == NULL)
			return (DEBUFFER_STATE_ERROR);
		free(c->line);
	}
	if (ft_strncat(tmp, c->buf, next_segment_length) == NULL)
		return (DEBUFFER_STATE_ERROR);
	c->line = tmp;
	if (next_nl)
	{
		c->line_start = next_nl + 1;
		return (DEBUFFER_STATE_END_OF_LINE);
	}
	else
	{
		c->line_start = c->buf;
		return (DEBUFFER_STATE_END_OF_BUFFER);
	}
}


/*
**  The challenge of handling arbitrary buffer sizes:
**  If the buffer is smaller than the line
** 	--> then lines are split between multiple buffers
**  If the buffer is larger than the line
**  --> then there may be multiple lines in the buffer
**
**	
*/

static int				get_next_line_in_context(gnl_context_t *c)
{
	c->line_size = 0;
	while (c->debuffer_state != DEBUFFER_STATE_END_OF_LINE)
	{
		if (c->debuffer_state == DEBUFFER_STATE_END_OF_BUFFER ||
			c->debuffer_state == DEBUFFER_STATE_INIT)
		{
			c->read_return_value = read(c->fd, c->buf, BUFF_SIZE);
			if (c->read_return_value < 0)
				return (GET_NEXT_LINE_READ_ERROR);
			if (c->read_return_value == 0)
			{
				if (c->line)
				{
					free(c->line);
					c->line = NULL;
				}
				return (GET_NEXT_LINE_READ_COMPLETE);
			}
		}
		if ((c->debuffer_state = debuffer_line(c)) == DEBUFFER_STATE_ERROR)
			return (GET_NEXT_LINE_READ_ERROR);
	}
	return (GET_NEXT_LINE_READ_SUCCESS);
}

/*
**	Returns with 1 when '\n' is encountered, with 0 when read() returns 0, or
**  with -1 when an error occurs.
**
**	Creates new contexts for new file descriptors
**	Destroys contexts when EOF is reached, or when an error occurs
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
	return_value = get_next_line_in_context(c);
	*line = c->line;
	if (return_value == GET_NEXT_LINE_READ_ERROR ||
		return_value == GET_NEXT_LINE_READ_COMPLETE)
	{
		c = ft_lstremove(&contexts, c);
		free(c);
	}
	return (return_value);
}
