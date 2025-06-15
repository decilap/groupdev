/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:17:28 by ddecilap          #+#    #+#             */
/*   Updated: 2025/05/25 12:27:28 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dstp;
	unsigned const char	*srcp;

	if (dst == NULL || src == NULL)
		return (NULL);
	dstp = (unsigned char *)dst;
	srcp = (unsigned char *)src;
	while (n--)
		*dstp++ = *srcp++;
	return (dst);
}
