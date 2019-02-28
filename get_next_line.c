/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/02/27 14:30:35 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include "libft.h"

int get_next_line(const int fd, char **line)
{
	char	buf[BUFF_SIZE];
	size_t	number_of_bytes_read;
	
	ft_bzero(buf, BUFF_SIZE);
	number_of_bytes_read = BUFF_SIZE;
	while (number_of_bytes_read > 0)
	{
		number_of_bytes_read = read(fd, buf, BUFF_SIZE);
	}
	// read, malloc, free
	return 0;
}
