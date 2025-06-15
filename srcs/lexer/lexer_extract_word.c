/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extract_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 17:15:33 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Appends one character to result during quoted extraction.
 *
 * Handles backslash escaping for double quotes.
 *
 * @param line The input line.
 * @param i Pointer to current index.
 * @param result Current result string (will be updated).
 * @param quote The active quote character.
 * @return 1 on success, 0 on malloc failure.
 */
static int	append_quoted_char(const char *line, int *i, char **result,
	char quote)
{
	char	*add;
	char	*tmp;

	if (quote == '"' && line[*i] == '\\' && line[*i + 1])
	{
		add = ft_substr(line, *i + 1, 1);
		*i += 2;
	}
	else
	{
		add = ft_substr(line, *i, 1);
		(*i)++;
	}
	if (!add)
		return (0);
	tmp = *result;
	*result = ft_strjoin(*result, add);
	free(tmp);
	free(add);
	if (!(*result))
		return (0);
	return (1);
}

/**
 * @brief Extracts a quoted word (single or double quotes).
 *
 * @param line The input line.
 * @param i Pointer to current index.
 * @param quoted_out Pointer to store quoted state.
 * @param quote_char_out Pointer to store quote type.
 * @return Newly allocated extracted string.
 */
static char	*extract_quoted_word(const char *line, int *i, int *quoted_out,
	t_quote_state *quote_char_out)
{
	char			*result;
	char			quote;
	t_quote_state	qtype;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	quote = line[(*i)++];
	if (quote == '\'')
		qtype = Q_SINGLE_QUOTE;
	else
		qtype = Q_DOUBLE_QUOTE;
	if (quoted_out)
		*quoted_out = 1;
	if (quote_char_out)
		*quote_char_out = qtype;
	while (line[*i] && line[*i] != quote)
	{
		if (!append_quoted_char(line, i, &result, quote))
			return (NULL);
	}
	if (line[*i] == quote)
		(*i)++;
	return (result);
}

/**
 * @brief Extracts a single character while parsing unquoted words.
 *
 * Handles escaped characters if backslash is encountered.
 *
 * @param line The input line.
 * @param i Pointer to current index (modified inside).
 * @return Newly allocated string of size 1, or NULL on malloc failure.
 */
static char	*extract_unquoted_char(const char *line, int *i)
{
	char	*add;

	if (line[*i] == '\\' && line[*i + 1])
	{
		add = ft_substr(line, *i + 1, 1);
		*i += 2;
	}
	else
	{
		add = ft_substr(line, *i, 1);
		(*i)++;
	}
	return (add);
}

/**
 * @brief Extracts an unquoted word, stopping at special characters or spaces.
 *
 * Uses extract_unquoted_char() for each character.
 *
 * @param line The input line.
 * @param i Pointer to current index.
 * @return Newly allocated extracted string.
 */
static char	*extract_unquoted_word(const char *line, int *i)
{
	char	*result;
	char	*tmp;
	char	*add;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (is_unquoted_word_char(line, *i))
	{
		add = extract_unquoted_char(line, i);
		if (!add)
		{
			free(result);
			return (NULL);
		}
		tmp = result;
		result = ft_strjoin(result, add);
		free(tmp);
		free(add);
		if (!result)
			return (NULL);
	}
	return (result);
}

/**
 * @brief Extracts a word from the line, handling both quoted and unquoted forms.
 *
 * Updates quoted flags and quote types.
 *
 * @param line The input line.
 * @param i Pointer to current index.
 * @param quoted_out Pointer to store quoted state.
 * @param quote_char_out Pointer to store quote type.
 * @return Newly allocated word string.
 */
char	*extract_word(const char *line, int *i, int *quoted_out,
	t_quote_state *quote_char_out)
{
	if (line[*i] == '\'' || line[*i] == '"')
		return (extract_quoted_word(line, i, quoted_out, quote_char_out));
	if (quoted_out)
		*quoted_out = 0;
	if (quote_char_out)
		*quote_char_out = Q_NONE;
	return (extract_unquoted_word(line, i));
}
