/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 17:59:26 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Creates and adds a logical token to the token list.
 *
 * Handles memory allocation and token addition.
 *
 * @param tokens Pointer to the token list.
 * @param str The string representation of the operator.
 * @param type The token type (TOKEN_AND, TOKEN_OR, TOKEN_PIPE).
 * @return 1 on success, 0 on failure.
 */
static int	create_logical_token(t_token **tokens, const char *str, int type)
{
	char			*op;
	t_token_data	data;

	op = ft_strdup(str);
	if (!op)
		return (0);
	data = (t_token_data){op, type, 0, Q_NONE};
	if (!add_token(tokens, data))
	{
		free(op);
		return (0);
	}
	free(op); // ✅ Corrige la fuite
	return (1);
}


/**
 * @brief Handles logical operators (&&, ||, |) during lexing.
 *
 * Fully uses lexer context (t_lexer_ctx).
 *
 * @param ctx The lexer context.
 * @return 1 if a logical operator was handled, 0 otherwise.
 */
int	lexer_handle_logical(t_lexer_ctx *ctx)
{
	if (ctx->line[ctx->i] == '&' && ctx->line[ctx->i + 1] == '&')
	{
		if (!create_logical_token(&ctx->tokens, "&&", TOKEN_AND))
			return (0);
		ctx->i += 2;
		return (1);
	}
	else if (ctx->line[ctx->i] == '|' && ctx->line[ctx->i + 1] == '|')
	{
		if (!create_logical_token(&ctx->tokens, "||", TOKEN_OR))
			return (0);
		ctx->i += 2;
		return (1);
	}
	else if (ctx->line[ctx->i] == '|')
	{
		if (!create_logical_token(&ctx->tokens, "|", TOKEN_PIPE))
			return (0);
		ctx->i++;
		return (1);
	}
	return (0);
}

/**
 * @brief Processes one token at the current position.
 *
 * Handles redirection, logical operators and word tokens.
 * Also handles token joining based on previous tokens.
 *
 * @param ctx The lexer context.
 */
static void	process_one_token(t_lexer_ctx *ctx)
{
	while (ft_isspace(ctx->line[ctx->i]))
		ctx->i++;
	if (!ctx->line[ctx->i])
		return ;
	ctx->start = ctx->i;
	if (ctx->line[ctx->i] == '>' || ctx->line[ctx->i] == '<')
		lexer_handle_redir(ctx);
	else if (lexer_handle_logical(ctx))
		return ;
	else
		lexer_handle_word(ctx);
	ctx->end = ctx->i;
	ctx->curr = ctx->tokens;
	while (ctx->curr && ctx->curr->next)
		ctx->curr = ctx->curr->next;
	if (ctx->prev && ctx->curr
		&& (ctx->prev->type == TOKEN_WORD || ctx->prev->type == TOKEN_SUBSHELL)
		&& (ctx->curr->type == TOKEN_WORD || ctx->curr->type == TOKEN_SUBSHELL)
		&& is_token_joined(ctx->line, ctx->last_token_end, ctx->start))
		ctx->prev->joined = 1;
	else if (ctx->prev)
		ctx->prev->joined = 0;
	ctx->last_token_end = ctx->end;
	ctx->prev = ctx->curr;
}

/**
 * @brief Initializes the lexer context before starting tokenization.
 *
 * This function sets all the fields of the lexer context to their initial state,
 * including the input line, current index, token list,
 * and internal tracking variables.
 *
 * @param ctx The lexer context structure to initialize.
 * @param line The input command line to tokenize.
 */
void	lexer_init_context(t_lexer_ctx *ctx, const char *line)
{
	ctx->line = line;
	ctx->i = 0;
	ctx->start = 0;
	ctx->end = 0;
	ctx->last_token_end = 0;
	ctx->tokens = NULL;
	ctx->prev = NULL;
	ctx->curr = NULL;
}

/**
 * @brief Main lexer function that tokenizes the entire input line.
 *
 * @param line The input string to tokenize.
 * @return The generated token list.
 */
t_token	*lexer(const char *line)
{
	t_lexer_ctx	ctx;

	lexer_init_context(&ctx, line);
	while (line[ctx.i])
		process_one_token(&ctx);
	return (ctx.tokens);
}
