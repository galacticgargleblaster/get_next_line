/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/01 10:13:56 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** The first parameter is the file descriptor that will be used to read.
** The second parameter is the address of a pointer to a character that will
** be used to save the line read from the file descriptor.
** The return value can be 1, 0 or -1 depending on whether a line has been
** read, when the reading has been completed, or if an error has happened
** respectively.
*/

/*
** How big of a buffer should I allocate initially?  Probs doesn't matter.
** Make it work with bufsize 1 and bignum of mallocs
**
** Static is a function-scoped global.  this sucks.
** point to a struct that stores state, what file is being read from, etc.
**
** data structure may be a dictionary of {fildes: context}
** or maybe just a LinkedList of contexts, each with a fildes.
**
** static variables are initialized to zero
** https://port70.net/~nsz/c/c11/n1570.html#6.7.9p10
**
*/

#ifndef _GET_NEXT_LINE_H_
# define _GET_NEXT_LINE_H_

#include <unistd.h>

#ifndef BUFF_SIZE
#define BUFF_SIZE 1
#endif

#define GET_NEXT_LINE_READ_SUCCESS 1
#define GET_NEXT_LINE_READ_COMPLETE 0
#define GET_NEXT_LINE_READ_ERROR -1

/*
**	A context is used to hold read state on every file descriptor
**	that get_next_line is called on.
*/

typedef struct	gnl_context_s
{
	int			fd;
	char		*line;
	char		buf[BUFF_SIZE];
	size_t		line_size;
	ssize_t		read_return_value;
}				gnl_context_t;

int get_next_line(const int fd, char **line);

#endif
