/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/02/28 11:01:54 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"
#include "libft/includes/libft.h"

static gnl_context_t	*get_existing_context(const int fd, t_list *elt)
{
	while (elt && elt->content)
	{
		if (((gnl_context_t*)(elt->content))->fildes == fd)
			return (elt->content);
		elt = elt->next;
	}
	return (NULL);
}

static gnl_context_t	*__gnl_context_init__(const int fd)
{
	gnl_context_t	*context;

	context = malloc(sizeof(gnl_context_t));
	if (context == NULL)
		return (NULL);
	ft_bzero(context->buf, BUFF_SIZE);
	context->fildes = fd;
	context->read_return_value = BUFF_SIZE;
	return (context);
}

static void				__gnl_context_del__(gnl_context_t *context)
{
	free(context->buf);
	free(context);
}


int						get_next_line(const int fd, char **line)
{
	static t_list	*contexts;
	gnl_context_t	*c;
	char			*tmp;

	c = get_existing_context(fd, contexts);
	if (c == NULL)
	{
		c = __gnl_context_init__(fd);
		contexts = ft_lstpush(contexts, c, sizeof(gnl_context_t));
	}
	
	while ((c->read_return_value = read(fd, c->buf, BUFF_SIZE)) > 0)
	{
		;
		c->line_size += (ft_strchr(c->buf, '\n') ? (size_t)(c->buf - ft_strchr(c->buf, '\n')) : BUFF_SIZE);
		tmp = ft_strnew(c->line_size);
		if (tmp == NULL ||
			(c->line ? (ft_strcpy(tmp, c->line) == NULL) : 0) ||
			ft_strncat(tmp, c->buf, c->line_size) == NULL) 
			return (GET_NEXT_LINE_READ_ERROR);
		free(c->line);
		c->line = tmp;
	}

	// read, malloc, free
	return 0;
}
