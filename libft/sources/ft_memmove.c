/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:37:05 by ddecilap          #+#    #+#             */
/*   Updated: 2025/02/18 11:36:42 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*pdst;
	const unsigned char	*pstr;

	pstr = (const unsigned char *)src;
	pdst = (unsigned char *)dst;
	if (pdst < pstr)
	{
		while (len--)
			*pdst++ = *pstr++;
	}
	else
	{
		pdst += len;
		pstr += len;
		while (len--)
			*--pdst = *--pstr;
	}
	return (dst);
}
