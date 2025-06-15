/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:00:41 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/11 10:19:56 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *c, int fd)
{
	if (!c || fd < 0)
		return ;
	while (*c)
	{
		safe_write(fd, c, 1);
		c++;
	}
	safe_write(fd, "\n", 1);
}
