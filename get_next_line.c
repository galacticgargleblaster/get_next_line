/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/03 16:11:47 by nkirkby          ###   ########.fr       */
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
	context->read_return_value = BUFF_SIZE;
	context->line_start = NULL;
	context->debuffer_state = DEBUFFER_STATE_HUNGRY;
	return (context);
}

/*
**	Copies bytes from the context's buffer into the context's line
**
**	Returns DEBUFFER_STATE_HUNGRY if there's more buffer to be eaten.
**  Returns DEBUFFER_STATE_END_OF_LINE if '\n' is reached
**	Returns DEBUFFER_STATE_NULL if line_start points to null
**  Returns DEBUFFER_STATE_ERROR if there's an issue
**
**  CASE 1: BUFF_SIZE [7] > len(line) [3]
**
** | f | o | o | \n| b | a | r |
**   ^
**   line_start, first call.  Returns DEBUFFER_STATE_END_OF_LINE
**
** | f | o | o | \n| b | a | r |
**                   ^
**                   line_start, second call, Returns DEBUFFER_STATE_HUNGRY (reached EOB)
**
** NULL
**  ^
**  line_start, third call, Returns DEBUFFER_STATE_NULL
**
**  CASE 2: BUFF_SIZE [1] < len(line) [3]
**
** | f |
**   ^
**   line_start, first call.  Returns DEBUFFER_STATE_NULL
**
** | \n|
**   ^
**   line_start, fourth call, Returns DEBUFFER_STATE_END_OF_LINE
**
** NULL
**  ^
**  line_start, eighth call, Returns DEBUFFER_STATE_NULL
**
**  CASE 3: BUFF_SIZE [1] < len(line) [3], no newline
**
** | f |
**   ^
**   line_start, first call.  Returns DEBUFFER_STATE_NULL
*/

#define REMAINING_BUFF_SIZE(c) (size_t)(BUFF_SIZE - (c->line_start - c->buf))
#define NEXT_NL(c) (ft_memchr(c->line_start, '\n', (REMAINING_BUFF_SIZE(c))))
#define BYTES_TO_NEXT_NL(c) ((char*)NEXT_NL(c) - c->line_start)

static int				debuffer(t_gnl_context *c)
{
	char				*tmp;
	char				*cpy;
	char				*next_nl;
	size_t				size;

	next_nl = NEXT_NL(c);
	size = (next_nl == NULL ? REMAINING_BUFF_SIZE(c) : BYTES_TO_NEXT_NL(c));
	if ((cpy = ft_strndup(c->line_start, size)) == NULL)
		return (DEBUFFER_STATE_ERROR);
	if (c->line)
	{
		if ((tmp = ft_strjoin(c->line, cpy)) == NULL)
			return (DEBUFFER_STATE_ERROR);
		free(cpy);
		free(c->line);
		c->line = tmp;
	}
	else
		c->line = cpy;
	c->line_start = (REMAINING_BUFF_SIZE(c) > size ? next_nl : NULL);
	if (next_nl)
		return (DEBUFFER_STATE_END_OF_LINE);
	if (c->line_start == NULL)
		return (DEBUFFER_STATE_UNCERTAIN);
	return (DEBUFFER_STATE_HUNGRY);
}

/*
**
*/

#define IN_RANGE(ptr, c) (((ptr) > (c->buf)) && ((ptr) < (c->buf + BUFF_SIZE)))

static int				get_next_line_in_context(t_gnl_context *c)
{
	if (c->debuffer_state == DEBUFFER_STATE_END_OF_LINE)
	{
		c->line_start = (IN_RANGE(c->line_start + 1, c) ?
						c->line_start + 1 : NULL);
		if (c->line_start == NULL)
			c->debuffer_state = DEBUFFER_STATE_HUNGRY;
	}
	while (1)
	{
		if (c->debuffer_state == DEBUFFER_STATE_HUNGRY ||
			c->debuffer_state == DEBUFFER_STATE_UNCERTAIN)
		{
			if ((c->read_return_value = read(c->fd, c->buf, BUFF_SIZE)) < 0)
				return (GET_NEXT_LINE_READ_ERROR);
			else if (c->read_return_value == 0 && c->debuffer_state == DEBUFFER_STATE_UNCERTAIN)
			{
				c->debuffer_state = DEBUFFER_STATE_HUNGRY;
				return (GET_NEXT_LINE_READ_SUCCESS);
			}
			else if (c->read_return_value == 0)
				return (GET_NEXT_LINE_READ_COMPLETE);
			c->line_start = c->buf;
		}
		if ((c->debuffer_state = debuffer(c)) == DEBUFFER_STATE_ERROR)
			return (GET_NEXT_LINE_READ_ERROR);
		if (c->debuffer_state == DEBUFFER_STATE_END_OF_LINE)
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

	if (BUFF_SIZE < 1 || fd < 0)
		return (GET_NEXT_LINE_READ_ERROR);
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
