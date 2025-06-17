/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_arr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:10:51 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/17 13:25:22 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*copy_strings(char *dest, const char *src1, const char *src2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (src1[j])
		dest[i++] = src1[j++];
	j = 0;
	while (src2[j])
		dest[i++] = src2[j++];
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin_array(char **line, char *buff)
{
	char	*tmp;
	size_t	len_line;
	size_t	len_buff;

	if (!line || !buff)
		return (NULL);
	len_line = ft_strlen(*line);
	len_buff = ft_strlen(buff);
	tmp = malloclen_line + len_buff + 1);
	if (!tmp)
		return (NULL);
	copy_strings(tmp, *line, buff);
	free(*line);
	*line = tmp;
	return (tmp);
}
