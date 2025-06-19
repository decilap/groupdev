/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:35:24 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 14:56:32 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Main parsing loop for token list to command chain.
 *
 * Iterates through all tokens, handles heredoc, subshells, words, redirections,
 * and operators, building a chain of command structs.
 *
 * @param ctx    The parsing context (tokens, args, command head, etc).
 * @param shell  The shell context for environment, status, etc.
 */
static void	handle_operator_token(t_token *tok, t_parse_ctx *ctx)
{
	t_cmd	*new;

	if (tok->type == TOKEN_PIPE || tok->type == TOKEN_AND
		|| tok->type == TOKEN_OR)
	{
		ctx->args[ctx->arg_i] = NULL;
		finalize_cmd_args(ctx);
		ctx->curr->next_type = tok->type;
		new = setup_new_cmd();
		ctx->curr->next = new;
		new->prev = ctx->curr;
		new->prev_type = tok->type;
		ctx->curr = new;
		free(ctx->args);
		free(ctx->quote_chars);
		ctx->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
		ctx->quote_chars = malloc(sizeof(t_quote_state) * MAX_CMD_ARGS);
		if (!ctx->args || !ctx->quote_chars)
			exit_error("malloc( failed");
		ft_memset(ctx->args, 0, sizeof(char *) * MAX_CMD_ARGS);
		ft_memset(ctx->quote_chars, 0, sizeof(t_quote_state) * MAX_CMD_ARGS);
		ctx->arg_i = 0;
		ctx->quote_i = 0;
	}
}

/**
 * @brief Processes a single token in the parsing loop.
 * Handles heredocs, subshells, words, redirections and operators.
 * 
 * @param ctx The parsing context.
 * @param shell The shell context.
 * @param multi_data Multi-line heredoc data if any.
 * @return 1 if processing should continue to next iteration, 0 otherwise.
 */
static int	process_current_token(t_parse_ctx *ctx, t_shell *shell,
	t_token *multi_data)
{
	int	heredoc_status;

	heredoc_status = handle_heredoc_token(&ctx->tok,
			ctx->shell, ctx->curr, multi_data);
	if (heredoc_status == -1)
		return (-1);
	if (heredoc_status == 1)
		return (1);
	if (process_token_subshell(ctx))
		return (1);
	if (ctx->tok->type == TOKEN_WORD)
		ctx->tok = handle_word_token_block(ctx);
	handle_redirection_token(&ctx->tok, ctx->curr, shell);
	handle_operator_token(ctx->tok, ctx);
	return (0);
}

/**
 * @brief Main parsing loop that iterates through all tokens.
 * Delegates processing to process_current_token().
 *
 * @param ctx The parsing context.
 * @param shell The shell context.
 * @param multi_data Multi-line heredoc data.
 */
static void	process_token_loop(t_parse_ctx *ctx, t_shell *shell,
	t_token *multi_data)
{
	int	result;

	while (ctx->tok)
	{
		if (!ctx->curr)
		{
			ctx->curr = setup_new_cmd();
			if (!ctx->head)
				ctx->head = ctx->curr;
		}
		result = process_current_token(ctx, shell, multi_data);
		if (result == -1)
			return ;
		if (result == 1)
			continue ;
		ctx->tok = ctx->tok->next;
	}
}

/**
 * @brief Initializes the parsing context (allocations + initial state).
 *
 * Allocates memory for args and quote_chars, sets default values.
 *
 * @param ctx The parsing context structure to initialize.
 * @param tok The head of the token list.
 * @param shell The shell structure.
 */
static void	init_parse_context(t_parse_ctx *ctx, t_token *tok, t_shell *shell)
{
	ctx->head = NULL;
	ctx->curr = NULL;
	ctx->tok = tok;
	ctx->shell = shell;
	ctx->arg_i = 0;
	ctx->quote_i = 0;
	ctx->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
	ctx->quote_chars = malloc(sizeof(t_quote_state) * MAX_CMD_ARGS);
	if (!ctx->args || !ctx->quote_chars)
		exit_error("malloc( args or quote_chars failed");
	ft_memset(ctx->args, 0, sizeof(char *) * MAX_CMD_ARGS);
	ft_memset(ctx->quote_chars, 0, sizeof(t_quote_state) * MAX_CMD_ARGS);
}

/**
 * @brief Parses tokenized input into a linked list of commands.
 *
 * @param tok The head of the token list.
 * @param shell The shell context.
 * @param multi_data Multi-line heredoc data.
 * @return Head of the parsed t_cmd list, or NULL on failure.
 */
t_cmd	*parse_tokens(t_token *tok, t_shell *shell, t_token *multi_data)
{
	int			i;
	t_parse_ctx	ctx;

	init_parse_context(&ctx, tok, shell);
	process_token_loop(&ctx, shell, multi_data);
	if (ctx.curr && ctx.arg_i > 0)
	{
		finalize_cmd_args(&ctx);
		i = 0;
		while (i < ctx.arg_i)
		{
			if (ctx.args[i])
			{
				free(ctx.args[i]);
				ctx.args[i] = NULL;
			}
			i++;
		}
	}
	free(ctx.args);
	free(ctx.quote_chars);
	ctx.arg_i = 0;
	ctx.quote_i = 0;
	return (ctx.head);
}
