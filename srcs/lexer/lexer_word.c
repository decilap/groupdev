/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 18:03:06 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Extracts a word from the input, trims it, and updates word context.
 *
 * Calls extract_word() to parse the word, applies trimming,
 * and fills the word context.
 *
 * @param line The input command line.
 * @param i Pointer to the current index.
 * @param word The word context structure to update.
 * @return 0 on success, 1 on failure.
 */
static int	extract_and_trim_word(const char *line, int *i, t_word_ctx *word)
{
	char	*raw_word;

	raw_word = extract_word(line, i, &word->quoted, &word->quote_char);
	if (!raw_word)
		return (1);
	word->trimmed = ft_strtrim(raw_word, "\t\n\r");
	free(raw_word);
	if (!word->trimmed)
		return (1);
	return (0);
}

/**
 * @brief Creates and adds a token for a word or subshell.
 *
 * If the word matches "__SUBSHELL_", it's marked as subshell,
 * otherwise as regular word.
 *
 * @param tokens The token list.
 * @param trimmed The trimmed word string.
 * @param quoted Whether the word was quoted.
 * @param quote_char The type of quote applied.
 * @return The added token, or NULL on failure.
 */
static t_token	*add_word_token(t_token **tokens, char *trimmed,
	int quoted, t_quote_state quote_char, t_shell *shell)
{
	t_token_data	data;

	if (ft_strncmp(trimmed, "__SUBSHELL_", 11) == 0)
		data = (t_token_data){trimmed, TOKEN_SUBSHELL, quoted, quote_char};
	else
		data = (t_token_data){trimmed, TOKEN_WORD, quoted, quote_char};
	return (add_token(tokens, data, shell));
}

/**
 * @brief Updates the joined flag if the current token should
 * be merged with the previous one.
 *
 * Checks if the previous and current tokens are adjacent without spaces
 * and are both words or subshells.
 *
 * @param line The input command line.
 * @param start_pos Start position of the current token.
 * @param end_pos End position of the current token.
 * @param tokens The token list.
 * @param tok The newly added token.
 */
static void	handle_joined_flag(t_lexer_ctx *ctx, t_token *tok)
{
	t_token	*prev;

	if (tok && ctx->tokens && ctx->tokens->next)
	{
		prev = ctx->tokens;
		while (prev->next && prev->next != tok)
			prev = prev->next;
		if (is_token_joined(ctx->line, ctx->start, ctx->end))
			prev->joined = 1;
	}
}

/**
 * @brief Creates and adds a dollar-quoted token after trimming.
 *
 * Handles trimming, token creation, and joined flag detection.
 *
 * @param line The input line.
 * @param i Pointer to the current index.
 * @param start_pos Start index of the token.
 * @param quoted_word The extracted (untrimmed) quoted string.
 * @param tokens The token list.
 * @return 0 on success, 1 on failure.
 */
/**
 * @brief Trims, adds the token and updates joined flag for dollar-quoted words.
 *
 * @param ctx The lexer context.
 * @param quoted_word The extracted quoted word (untrimmed).
 * @return 0 on success, 1 on failure.
 */
int	add_trimmed_dollar_token(t_lexer_ctx *ctx, char *quoted_word, t_shell *shell)
{
	char			*trimmed;
	t_token_data	data;
	t_token			*tok;
	t_token			*prev;

	trimmed = ft_strtrim(quoted_word, "\t\n\r");
	free(quoted_word);
	if (!trimmed)
		return (1);
	data = (t_token_data){trimmed, TOKEN_WORD, 1, Q_DOUBLE_QUOTE};
	tok = add_token(&ctx->tokens, data, shell);
	if (!tok)
		return (1);
	if (tok && ctx->tokens && ctx->tokens->next)
	{
		prev = ctx->tokens;
		while (prev->next && prev->next != tok)
			prev = prev->next;
		if (is_token_joined(ctx->line, ctx->start, ctx->i))
			prev->joined = 1;
	}
	return (0);
}

/**
 * @brief Handles word tokens in the lexer.
 *
 * Detects if the word starts with special $"..." construct,
 * otherwise extracts the word normally, trims it, and adds it as a token.
 *
 * @param line The input command line.
 * @param i Pointer to the current index.
 * @param tokens The token list.
 */
void	lexer_handle_word(t_lexer_ctx *ctx, t_shell *shell)
{
	t_word_ctx	word;
	t_token		*tok;

	ctx->start = ctx->i;
	if (ctx->line[ctx->i] == '$' && ctx->line[ctx->i + 1] == '"')
	{
		if (handle_dollar_quote(ctx, shell))
			return ;
		return ;
	}
	if (extract_and_trim_word(ctx->line, &ctx->i, &word))
		return ;
	ctx->end = ctx->i;
	tok = add_word_token(&ctx->tokens, word.trimmed, word.quoted,
			word.quote_char, shell);
	handle_joined_flag(ctx, tok);
}
