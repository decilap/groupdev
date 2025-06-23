/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 18:04:05 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Checks if a character is a shell special symbol.
 *
 * @param c The character to test.
 * @return 1 if the character is a special symbol (|, <, >), 0 otherwise.
 */
static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * @brief Checks if two tokens are joined without any space between them.
 *
 * This function checks the characters between the end of the current token
 * and the start of the next token. If any whitespace is found, the tokens
 * are considered not joined.
 *
 * This is used to determine whether two adjacent tokens should be merged
 * (e.g. for cases like: echo$HOME or $""HOME).
 *
 * @param line The full input line.
 * @param curr_end The index where the current token ends.
 * @param next_start The index where the next token starts.
 * @return 1 if tokens are joined (no space between them), 0 otherwise.
 */
int	is_token_joined(const char *line, int curr_end, int next_start)
{
	while (curr_end < next_start)
	{
		if (ft_isspace(line[curr_end]))
			return (0);
		curr_end++;
	}
	return (1);
}

/**
 * @brief Checks whether the current character can be part of an unquoted word.
 *
 * Stops at whitespace, quotes, special operators, or other special tokens.
 *
 * @param line The input line.
 * @param i The current index.
 * @return 1 if character is valid for unquoted word, 0 otherwise.
 */
int	is_unquoted_word_char(const char *line, int i)
{
	if (!line[i])
		return (0);
	if (ft_isspace(line[i]))
		return (0);
	if (line[i] == '"' || line[i] == '\'')
		return (0);
	if (line[i] == '&' && line[i + 1] == '&')
		return (0);
	if (line[i] == '|' && line[i + 1] == '|')
		return (0);
	if (is_special(line[i]))
		return (0);
	if (line[i] == '$' && line[i + 1] == '"')
		return (0);
	return (1);
}

/**
 * @brief Extracts the content inside a $"..." quoted string.
 *
 * Allocates and builds the merged string from characters inside $"...".
 *
 * @param line The input command line.
 * @param i Pointer to current index (will be updated).
 * @return Newly allocated extracted string, or NULL on failure.
 */
static char	*extract_dollar_quote_content(const char *line, int *i)
{
	char	*quoted_word;
	char	*tmp;
	char	*add;

	quoted_word = ft_strdup("");
	if (!quoted_word)
		return (NULL);
	*i += 2;
	while (line[*i] && line[*i] != '"')
	{
		add = ft_substr(line, *i, 1);
		if (!add)
			return (free(quoted_word), NULL);
		tmp = quoted_word;
		quoted_word = ft_strjoin(quoted_word, add);
		free(tmp);
		free(add);
		if (!quoted_word)
			return (NULL);
		(*i)++;
	}
	if (line[*i] == '"')
		(*i)++;
	return (quoted_word);
}

/**
 * @brief Handles parsing of special $"..." quoted tokens.
 *
 * Extracts content, trims, and adds token to token list.
 *
 * @param line The input command line.
 * @param i Pointer to the current index.
 * @param tokens The token list.
 * @return 0 on success, 1 on failure.
 */
int	handle_dollar_quote(t_lexer_ctx *ctx, t_shell *shell)
{
	char	*quoted_word;

	ctx->start = ctx->i;
	quoted_word = extract_dollar_quote_content(ctx->line, &ctx->i);
	if (!quoted_word)
		return (1);
	if (add_trimmed_dollar_token(ctx, quoted_word, shell))
		return (1);
	return (0);
}
