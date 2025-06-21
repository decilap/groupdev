/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ry_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:35:42 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/21 18:48:28 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

const char	*handle_separator(const char **str, char **result)
{
	*result = ft_strdup("");
	(*str)++;
	return (*str);
}

static char	*ft_copy_word(const char *start, size_t len)
{
	char	*word;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, start, len);
	word[len] = '\0';
	return (word);
}

const char	*handle_word(const char **str, char c, char **result)
{
	const char	*start;

	start = *str;
	while (**str && **str != c)
		(*str)++;
	*result = ft_copy_word(start, *str - start);
	if (**str)
		(*str)++;
	return (*str);
}
