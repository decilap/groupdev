/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:38:27 by ddecilap          #+#    #+#             */
/*   Updated: 2025/05/25 12:38:14 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	copy_len;
	char	*pdst;

	if (!dst || !src)
		return (0);
	src_len = ft_strlen(src);
	pdst = dst;
	while (*pdst && (size_t)(pdst - dst) < dstsize)
		pdst++;
	dst_len = pdst - dst;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	copy_len = dstsize - dst_len - 1;
	if (copy_len > src_len)
		copy_len = src_len;
	ft_memcpy(pdst, src, copy_len);
	pdst[copy_len] = '\0';
	return (dst_len + src_len);
}
