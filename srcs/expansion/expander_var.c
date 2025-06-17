/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/17 13:42:41 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Appends a single character to the result string and advances the index.
 *
 * @param result Pointer to the result string (updated and reallocated).
 * @param c      The character to append.
 * @param i      Pointer to the input index (advanced).
 * @return 1 on success, 0 on allocation failure.
 */
int	expand_append_char(char **result, char c, size_t *input_index)
{
	char	tmp[2];
	char	*new_res;

	tmp[0] = c;
	tmp[1] = '\0';
	new_res = str_append(*result, tmp);
	if (!new_res)
	{
		free(*result);
		*result = NULL;
		return (0);
	}
	*result = new_res;
	(*input_index)++;
	return (1);
}

/**
 * @brief Expands a tilde at the start of a filename to $HOME.
 *
 * If the filename begins with '~' (alone or followed by '/'), replaces
 * it with the user's home directory.
 * Frees the original string and returns the expanded one.
 *
 * @param filename The input filename (malloc('ed, will be freed if expanded).
 * @param shell    The shell context (for environment lookup).
 * @return Newly allocated expanded filename, or the original
 * if no expansion was needed.
 */
char	*expand_tilde_filename(char *filename, t_shell *shell)
{
	size_t	dummy;
	char	*expanded;

	if (filename && filename[0] == '~' && (filename[1] == '/'
			|| filename[1] == '\0'))
	{
		dummy = 0;
		expanded = expand_init_result(filename, &dummy, shell);
		if (expanded)
		{
			free(filename);
			filename = expanded;
		}
	}
	return (filename);
}
