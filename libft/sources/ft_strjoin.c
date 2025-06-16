/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:10:51 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/16 22:22:38 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	int		s1_len;
	int		s2_len;
	int		join_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	join_len = s1_len + s2_len + 1;
	newstr = (char *)my_malloc(join_len * sizeof(char));
	if (!newstr)
		return (NULL);
	if (s1)
		ft_memcpy(newstr, s1, s1_len);
	if (s2)
		ft_memcpy(newstr + s1_len, s2, s2_len);
	newstr[s1_len + s2_len] = '\0';
	return (newstr);
}
