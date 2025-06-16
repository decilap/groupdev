/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:20:18 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/16 22:22:38 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Frees a result string and returns NULL.
 *
 * Utility for handling allocation failures and cleanup in the expander.
 *
 * @param result The string to free.
 * @return Always returns NULL.
 */
static t_token	*expand_free_ret(char *result)
{
	free(result);
	return (NULL);
}

/**
 * @brief Creates a new token from an expanded string result.
 *
 * Allocates and initializes a new t_token of type TOKEN_WORD.
 *
 * @param result The expanded string (will be used as token value).
 * @return Newly allocated token, or NULL on allocation failure.
 */
static t_token	*expand_token(char *result, int quoted,
					t_quote_state quote_char)
{
	t_token	*tok;

	tok = my_malloc(sizeof(t_token));
	if (!tok)
		return (expand_free_ret(result));
	tok->value = result;
	tok->type = TOKEN_WORD;
	tok->quoted = quoted;
	tok->quote_char = quote_char;
	tok->joined = 0;
	tok->next = NULL;
	return (tok);
}

/**
 * @brief Main loop for processing variable and quote expansion.
 *
 * Iterates over the input, expanding variables and handling quoting logic.
 * Uses the state to determine which expander to call at each position.
 *
 * @param ctx The expansion context (input, state, result, etc.).
 * @return 1 on success, 0 on error or allocation failure.
 */
static int	expand_variables_loop(t_expander *ctx)
{
	int	success;

	while (ctx->input[*(ctx->i)])
	{
		if (*(ctx->state) == Q_NONE)
			success = expand_none(ctx);
		else if (*(ctx->state) == Q_SINGLE_QUOTE)
			success = expand_single(ctx);
		else if (*(ctx->state) == Q_HEREDOC)
			success = expand_heredoc(ctx);
		else
			success = expand_double(ctx);
		if (!success)
			return (0);
	}
	return (1);
}

/**
 * @brief Expands variables and quotes in an input string for the shell.
 *
 * Sets up the expansion context and processes the string, handling
 * environment variables, special characters, and quote states.
 * Returns a new token containing the expanded result.
 *
 * @param input The input string to expand.
 * @param shell The shell context (for env variables and exit status).
 * @return Newly allocated token (with expanded value), or NULL on failure.
 */
t_token	*expand_variables(const char *input, t_shell *shell, int quoted,
	t_quote_state quote_char)
{
	char			*result;
	size_t			i;
	t_expander		ctx;
	t_quote_state	state;

	i = 0;
	result = expand_init_result(input, &i, shell);
	if (!result)
		return (NULL);
	state = quote_char;
	ctx.input = input;
	ctx.shell = shell;
	ctx.result = &result;
	ctx.state = &state;
	ctx.i = &i;
	if (!expand_variables_loop(&ctx))
		return (expand_free_ret(result));
	return (expand_token(result, quoted, quote_char));
}

/**
 * @brief Expands variables inside a heredoc input.
 * 
 * This function prepares the expander context and processes the heredoc input 
 * with correct handling of quotes and environment variables.
 * 
 * - If the heredoc is unquoted, variables like $VAR and $? are expanded.
 * - If the heredoc is quoted, no expansion occurs (depending on quote_char).
 * 
 * @param input        The original heredoc string.
 * @param shell        The shell context (for environment access).
 * @param quoted       Whether the heredoc was quoted.
 * @param quote_char   The type of quote used (single or double).
 * @return A token list containing the expanded heredoc string,
 * or NULL on failure.
 */
t_token	*expand_variables_heredoc(const char *input, t_shell *shell, int quoted,
		t_quote_state quote_char)
{
	char			*result;
	size_t			i;
	t_expander		ctx;
	t_quote_state	state;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	state = quote_char;
	ctx.input = input;
	ctx.shell = shell;
	ctx.result = &result;
	ctx.state = &state;
	ctx.i = &i;
	if (!expand_variables_loop(&ctx))
		return (expand_free_ret(result));
	return (expand_token(result, quoted, quote_char));
}
