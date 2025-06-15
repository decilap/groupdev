/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:39:31 by ddecilap          #+#    #+#             */
/*   Updated: 2025/05/25 12:32:46 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*end;
	int		s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	end = (char *)s + s_len;
	if (c == '\0')
		return ((char *)end);
	while (end >= s)
	{
		if (*end == (char)c)
			return (end);
		end--;
	}
	return (NULL);
}
