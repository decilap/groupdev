/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_join_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:10:51 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/19 14:33:30 by ddecilap         ###   ########.fr       */
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
	tmp = malloc(len_line + len_buff + 1);
	if (!tmp)
		return (NULL);
	copy_strings(tmp, *line, buff);
	free(*line);
	*line = tmp;
	return (tmp);
}
