/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/02/27 21:14:50 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include "libft.h"

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
	context->fildes = fd;
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
	gnl_context_t	*context;

	context = get_existing_context(fd, contexts);
	if (context == NULL)
		contexts = ft_lstpush(contexts, __gnl_context_init__(fd), sizeof(gnl_context_t));
	

	buf = ft_memalloc(BUFF_SIZE);
	number_of_bytes_read = BUFF_SIZE;
	while (number_of_bytes_read > 0)
	{
		number_of_bytes_read = read(fd, buf, BUFF_SIZE);
	}

	// read, malloc, free
	return 0;
}
