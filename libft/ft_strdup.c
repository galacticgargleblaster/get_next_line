/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkirkby <nkirkby@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 09:01:14 by nkirkby           #+#    #+#             */
/*   Updated: 2019/02/28 15:06:12 by nkirkby          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** if malloc() fails, it will set errno to ENOMEM and return a NULL pointer.
*/

char	*ft_strdup(const char *src)
{
	return (ft_strndup(src, ft_strlen(src)));
}
