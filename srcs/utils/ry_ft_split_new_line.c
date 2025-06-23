/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ry_ft_split_new_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:56:53 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 13:15:03 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

static void	ft_free_array(char **result)
{
	int	i;

	i = 0;
	if (!result)
		return ;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static int	ft_count_word(const char *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count + 1);
}

static int	ft_split_string(const char *str, char c, char **result)
{
	int	i;

	i = 0;
	while (*str)
	{
		if (*str != c)
			str = handle_word(&str, c, &result[i]);
		else
		{
			result[i] = ft_strdup("");
			if (!result[i])
				return (0);
			i++;
			str++;
		}
		if (!result[i])
			return (0);
		i++;
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split_new_line(const char *str, char c, t_shell *shell)
{
	char	**result;
	int		word_count;

	if (!str)
		return (NULL);
	word_count = ft_count_word(str, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!ft_split_string(str, c, result))
	{
		ft_free_array(result);
		return (NULL);
	}
	result[word_count] = (NULL);
	shell->lines = result;
	return (result);
}
