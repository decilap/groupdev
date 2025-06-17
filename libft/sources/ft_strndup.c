/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:38:27 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/17 13:42:41 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char		*dst;
	char		*new;
	size_t		len;
	const char	*src;

	src = s;
	len = 0;
	while (*src && len < n)
	{
		src++;
		len++;
	}
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	dst = new;
	while (len--)
		*dst++ = *s++;
	*dst = '\0';
	return (new);
}
