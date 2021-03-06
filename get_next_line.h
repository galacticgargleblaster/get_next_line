/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/03/03 17:06:29 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>

/*
** Overriding preprocessor defines seems to be nontrivial per:
** https://stackoverflow.com/questions/18483166/
** override-macro-from-the-command-line
**
** So I wrap the definition in #ifndef/#endif.
** Which begs the question, how does Moulinette change this value?
** Surely not with sed I hope.
*/

# ifndef BUFF_SIZE
#  define BUFF_SIZE 10
# endif

# define GET_NEXT_LINE_READ_SUCCESS 1
# define GET_NEXT_LINE_READ_COMPLETE 0
# define GET_NEXT_LINE_READ_ERROR -1

typedef enum			e_debuffer_state
{
	ERROR = -1,
	HUNGRY,
	END_OF_LINE,
	UNCERTAIN
}						t_debuffer_state;

/*
**	A context is used to hold read state on every file descriptor
**	that get_next_line is called with.
**
**	fd - file descriptor
**	line - the line to be returned
**	buf - the buffer
**	start_idx - points to the next non-null character in the buffer
**               that follows a newline.  If no such character exists, then it
**				 points to NULL.
**	read_returned - the value returned by the systemcall read()
*/

typedef struct			s_gnl_context
{
	int					fd;
	char				*line;
	char				buf[BUFF_SIZE];
	char				*start_idx;
	ssize_t				read_returned;
	t_debuffer_state	debuffer_state;
}						t_gnl_context;

int						get_next_line(const int fd, char **line);

#endif
