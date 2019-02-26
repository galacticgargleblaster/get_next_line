/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 11:31:08 by nkirkby           #+#    #+#             */
/*   Updated: 2019/02/26 10:08:15 by nkirkby          ###   ########.fr       */
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

#ifndef _GET_NEXT_LINE_H_
# define _GET_NEXT_LINE_H_

#ifndef BUFF_SIZE
#define BUFF_SIZE 1
#endif

int get_next_line(const int fd, char **line);

#endif
