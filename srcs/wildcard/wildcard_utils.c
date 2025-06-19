/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 15:10:57 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Checks if a string matches a wildcard pattern.
 *
 * Supports '*' (matches any sequence, including empty) and '?' 
 * (matches any single character).
 * Recursively matches the pattern against the string.
 *
 * @param pattern The wildcard pattern.
 * @param str     The string to match.
 * @return 1 if the pattern matches the string, 0 otherwise.
 */
int	match_pattern(const char *pattern, const char *str)
{
	if (!*pattern && !*str)
		return ((1));
	if (*pattern == '*')
	{
		while (*str)
		{
			if (match_pattern(pattern + 1, str))
				return (1);
			str++;
		}
		return (match_pattern(pattern + 1, str));
	}
	if (*pattern == *str || (*pattern != '\0' && *pattern == '?'))
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

/**
 * @brief Sorts an array of strings in ascending ASCII order.
 *
 * Uses a simple bubble sort algorithm for sorting.
 *
 * @param array Array of strings to sort.
 * @param n     Number of strings in the array.
 */
void	sort_ascii(char **array, int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < n - 1)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(array[i], array[j]) > 0)
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Frees all strings in a wildcard context and the context's
 * results array.
 *
 * Should be called to avoid memory leaks after wildcard expansion.
 *
 * @param ctx Pointer to the wildcard context to free.
 */
void	free_wildcard_ctx(t_wildcard_ctx *ctx)
{
	int	i;

	i = 0;
	while (ctx->results[i])
		free(ctx->results[i++]);
	free(ctx->results);
}

/**
 * @brief Insert wildcard expansion results into a temporary token list.
 *
 * @param matches The list of expanded wildcard strings.
 * @param cur The current token being replaced.
 * @param inserted Pointer to the list where new tokens are inserted.
 * @return 1 on success, 0 on failure.
 */
static int	insert_wildcard_matches(char **matches, t_token *cur,
	t_token **inserted)
{
	int				i;
	t_token			*new_tok;
	t_token_data	data;

	i = 0;
	while (matches[i])
	{
		data = (t_token_data){
			matches[i],      // ⬅️ Pas besoin de ft_strdup
			cur->type,
			cur->quoted,
			cur->quote_char
		};
		new_tok = add_token(inserted, data);
		if (!new_tok)
			return (0);
		new_tok->joined = cur->joined;
		i++;
	}
	return (1);
}

/**
 * @brief Expands a wildcard in the given token and replaces it with a list of matched tokens.
 *
 * If the token contains a wildcard (e.g., '*') and matches are found, it replaces the token
 * with a list of tokens generated from the matching filenames. It ensures memory safety and 
 * returns the next token to continue iteration safely after the replacement.
 *
 * @param tokens Pointer to the head of the full token list.
 * @param cur The current token being processed (and potentially replaced).
 * @return Pointer to the next token after the inserted matches, or NULL on error.
 */

t_token *expand_wildcard_for_token(t_token **tokens, t_token *cur)
{
	char	**matches;
	t_token	*inserted;
	t_token	*last;

	inserted = NULL;
	matches = wildcard_expand(cur->value);
	if (!matches)
		return (NULL);
	if (!insert_wildcard_matches(matches, cur, &inserted))
	{
		ft_free_arr(matches);
		free_tokens(inserted);
		return (NULL);
	}
	replace_token_with_list(tokens, cur, inserted);
	ft_free_arr(matches);
	last = inserted;
	while (last && last->next)
		last = last->next;
	return (last ? last->next : NULL);
}

