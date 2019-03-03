/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/03 00:46:10 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"
#include "libft/includes/libft.h"

static t_gnl_context	*get_existing_context_for_fd(const int fd, t_list *elt)
{
	while (elt && elt->content)
	{
		if (((t_gnl_context*)(elt->content))->fd == fd)
			return (elt->content);
		elt = elt->next;
	}
	return (NULL);
}

static t_gnl_context	*get_new_context_for_fd(const int fd)
{
	t_gnl_context	*context;

	context = malloc(sizeof(t_gnl_context));
	if (context == NULL)
		return (NULL);
	ft_bzero(context->buf, BUFF_SIZE);
	context->line = NULL;
	context->fd = fd;
	context->line_size = 0;
	context->read_return_value = BUFF_SIZE;
	context->line_start = context->buf;
	context->debuffer_state = DEBUFFER_STATE_HUNGRY;
	return (context);
}

#define EFFECTIVE_BUFF_SIZE(c) (MIN(BUFF_SIZE, c->read_return_value))
#define BYTES_REMAINING(c) (EFFECTIVE_BUFF_SIZE(c) - (c->line_start - c->buf))
#define IN_RANGE(c, nl) (nl - c->buf < BUFF_SIZE)
#define EXACT_EOF(c) (c->line_size == (size_t)c->read_return_value && c->line_size < BUFF_SIZE)

/*
**	Returns DEBUFFER_STATE_HUNGRY if there's more buffer to be eaten
**  Returns DEBUFFER_STATE_SATISFIED if '\n' or EOB is reached
**  Returns DEBUFFER_STATE_ERROR if there's an issue
*/

static int				debuffer_line(t_gnl_context *c)
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
	if (ft_strncat(tmp, c->line_start, next_segment_length) == NULL)
		return (DEBUFFER_STATE_ERROR);
	c->line = tmp;
	c->line_start = (next_nl && IN_RANGE(c, next_nl) ? next_nl + 1 : c->buf);
	if (next_nl)
		return (DEBUFFER_STATE_SATISFIED);
	if (EXACT_EOF(c))
		return (DEBUFFER_STATE_EOF);
	return (DEBUFFER_STATE_HUNGRY);
}

static int				get_next_line_in_context(t_gnl_context *c)
{
	c->line_size = 0;
	while (1)
	{
		if (c->debuffer_state == DEBUFFER_STATE_HUNGRY ||
			c->debuffer_state == DEBUFFER_STATE_EOF)
		{
			c->read_return_value = read(c->fd, c->buf, BUFF_SIZE);
			if (c->read_return_value < 0)
				return (GET_NEXT_LINE_READ_ERROR);
			if (c->read_return_value == 0)
			{
				free(c->line);
				c->line = NULL;
				return (GET_NEXT_LINE_READ_COMPLETE);
			}
		}
		if ((c->debuffer_state = debuffer_line(c)) == DEBUFFER_STATE_ERROR)
			return (GET_NEXT_LINE_READ_ERROR);
		if (c->debuffer_state == DEBUFFER_STATE_HUNGRY)
			continue;
		return (GET_NEXT_LINE_READ_SUCCESS);
	}
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
	t_gnl_context	*c;
	int				return_value;

	*line = NULL;
	c = get_existing_context_for_fd(fd, contexts);
	if (c == NULL)
	{
		c = get_new_context_for_fd(fd);
		if (c == NULL)
			return (GET_NEXT_LINE_READ_ERROR);
		contexts = ft_lstpush(contexts, c);
	}
	return_value = get_next_line_in_context(c);
	*line = c->line;
	c->line = NULL;
	if (return_value == GET_NEXT_LINE_READ_ERROR ||
		return_value == GET_NEXT_LINE_READ_COMPLETE)
	{
		c = ft_lstremove(&contexts, c);
		free(c);
	}
	return (return_value);
}
