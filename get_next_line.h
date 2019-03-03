/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/02 22:20:16 by nkirkby          ###   ########.fr       */
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
** I started learning C inside a large company, and it shows.  5 functions
** isn't enough.  Everything should be smaller, maintainable, and explainable.
**
** I can remember browsing some STM32 motor control header files, and being
** disappointed as to how sparse they were. 
*/

#ifndef _GET_NEXT_LINE_H_
# define _GET_NEXT_LINE_H_

#include <unistd.h>

/*
** Overriding preprocessor defines seems to be nontrivial per:
** https://stackoverflow.com/questions/18483166/override-macro-from-the-command-line
**
** So I wrap the definition in #ifndef/#endif.
** Which begs the question, how does Moulinette change this value?
** Surely not with sed I hope. 
*/

#ifndef BUFF_SIZE
#define BUFF_SIZE 1
#endif

#define GET_NEXT_LINE_READ_SUCCESS 1
#define GET_NEXT_LINE_READ_COMPLETE 0
#define GET_NEXT_LINE_READ_ERROR -1

typedef enum	debuffer_state {
	DEBUFFER_STATE_ERROR = -1,
	DEBUFFER_STATE_HUNGRY,
	DEBUFFER_STATE_SATISFIED
}				DEBUFFER_STATE;

/*
**	A context is used to hold read state on every file descriptor
**	that get_next_line is called with.
**
**	fd - file descriptor
**	line - the line to be returned
**	buf - the buffer
**	line_start - points to the next non-null character in the buffer that follows a newline
**	read_return_value - the value returned by the systemcall read()
*/

typedef struct		gnl_context_s
{
	int				fd;
	char			*line;
	size_t			line_size;
	char			buf[BUFF_SIZE];
	char 			*line_start;
	ssize_t			read_return_value;
	DEBUFFER_STATE	debuffer_state;
}					gnl_context_t;

int					get_next_line(const int fd, char **line);

#endif
