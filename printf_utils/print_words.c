/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 23:24:31 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/09 17:26:03 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	print_string(va_list ap)
{
	char	*str;

	str = va_arg(ap, char *);
	if (!str)
	{
		ft_putstr_fd("(null)", 2);
		return (6);
	}
	ft_putstr_fd(str, 2);
	return (ft_strlen(str));
}

int	print_char(va_list ap)
{
	char	c;

	c = va_arg(ap, int);
	ft_putchar_fd(c, 1);
	return (1);
}
