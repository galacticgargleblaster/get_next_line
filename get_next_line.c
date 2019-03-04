/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/03 20:44:42 by nkirkby          ###   ########.fr       */
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
	context->read_returned = BUFF_SIZE;
	context->start_idx = NULL;
	context->debuffer_state = HUNGRY;
	return (context);
}

/*
**	Copies bytes from the context's buffer into the context's line
**
**	Returns DEBUFFER_STATE_HUNGRY if there's more buffer to be eaten.
**  Returns DEBUFFER_STATE_END_OF_LINE if '\n' is reached
**	Returns DEBUFFER_STATE_UNCERTAIN if the line may continue in the next buffer
**  Returns DEBUFFER_STATE_ERROR if there's an issue
**
**  CASE 1: BUFF_SIZE [7] > len(line) [3]
**
** | f | o | o | \n| b | a | r |
**   ^
**   start_idx, first call.  Returns DEBUFFER_STATE_END_OF_LINE
**
** | f | o | o | \n| b | a | r |
**                   ^
**                   start_idx, second call, Returns DEBUFFER_STATE_UNCERTAIN
**
** NULL
**  ^
**  start_idx, third call, Returns DEBUFFER_STATE_NULL
**
**  CASE 2: BUFF_SIZE [1] < len(line) [3]
**
** | f |
**   ^
**   start_idx, first call.  Returns DEBUFFER_STATE_NULL
**
** | \n|
**   ^
**   start_idx, fourth call, Returns DEBUFFER_STATE_END_OF_LINE
**
** NULL
**  ^
**  start_idx, eighth call, Returns DEBUFFER_STATE_NULL
**
**  CASE 3: BUFF_SIZE [1] < len(line) [3], no newline
**
** | f |
**   ^
**   start_idx, first call.  Returns DEBUFFER_STATE_NULL
*/

#define REMAINING_BUFF_SIZE(c) (size_t)(BUFF_SIZE - (c->start_idx - c->buf))
#define NEXT_NL(c) (ft_memchr(c->start_idx, '\n', (REMAINING_BUFF_SIZE(c))))
#define BYTES_TO_NEXT_NL(c) ((char*)NEXT_NL(c) - c->start_idx)

static int				debuffer(t_gnl_context *c)
{
	char				*tmp;
	char				*cpy;
	char				*next_nl;
	size_t				size;

	next_nl = NEXT_NL(c);
	size = (next_nl == NULL ? REMAINING_BUFF_SIZE(c) : BYTES_TO_NEXT_NL(c));
	if ((cpy = ft_strndup(c->start_idx, size)) == NULL)
		return (ERROR);
	if (c->line)
	{
		if ((tmp = ft_strjoin(c->line, cpy)) == NULL)
			return (ERROR);
		free(cpy);
		free(c->line);
		c->line = tmp;
	}
	else
		c->line = cpy;
	c->start_idx = (REMAINING_BUFF_SIZE(c) > size ? next_nl : NULL);
	if (next_nl)
		return (END_OF_LINE);
	if (c->start_idx == NULL)
		return (UNCERTAIN);
	return (HUNGRY);
}

#define LAST_CHAR(c) (c->buf + c->read_returned)
#define IN_BUFF(ptr, c) ((ptr >= c->buf) && (ptr < LAST_CHAR(c)))

static int				get_next_line_in_context(t_gnl_context *c)
{
	if (c->debuffer_state == END_OF_LINE)
	{
		c->start_idx = (IN_BUFF(c->start_idx + 1, c) ? c->start_idx + 1 : NULL);
		c->debuffer_state = (c->start_idx == NULL ? HUNGRY : c->debuffer_state);
	}
	while (1)
	{
		if (c->debuffer_state == HUNGRY || c->debuffer_state == UNCERTAIN)
		{
			ft_bzero(c->buf, BUFF_SIZE);
			if ((c->read_returned = read(c->fd, c->buf, BUFF_SIZE)) < 0)
				return (GET_NEXT_LINE_READ_ERROR);
			else if (c->read_returned == 0 && c->debuffer_state == UNCERTAIN)
			{
				c->debuffer_state = HUNGRY;
				return (GET_NEXT_LINE_READ_SUCCESS);
			}
			else if (c->read_returned == 0)
				return (GET_NEXT_LINE_READ_COMPLETE);
			c->start_idx = c->buf;
		}
		if ((c->debuffer_state = debuffer(c)) == ERROR)
			return (GET_NEXT_LINE_READ_ERROR);
		if (c->debuffer_state == END_OF_LINE)
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

	if (BUFF_SIZE < 1 || fd < 0 || line == NULL)
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
