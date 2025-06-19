/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:35:24 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 13:31:31 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

void	free_tmp_args(char **args, int count)
{
	int	i = 0;

	if (!args)
		return ;

	if (count == -1)
	{
		while (args[i])
		{
			if (DEBUG_MODE)
				fprintf(stderr, "[LOG] Freeing args[%d]: \"%s\" at %p\n", i, args[i], (void *)args[i]);
			free(args[i]);
			args[i] = NULL;
			i++;
		}
	}
	else
	{
		while (i < count)
		{
			if (args[i])
			{
				if (DEBUG_MODE)
					fprintf(stderr, "[LOG] Freeing args[%d]: \"%s\" at %p\n", i, args[i], (void *)args[i]);
				free(args[i]);
				args[i] = NULL;
			}
			i++;
		}
	}
}



/**
 * @brief Main parsing loop for token list to command chain.
 *
 * Iterates through all tokens, handles heredoc, subshells, words, redirections,
 * and operators, building a chain of command structs.
 *
 * @param ctx    The parsing context (tokens, args, command head, etc).
 * @param shell  The shell context for environment, status, etc.
 */
static void handle_operator_token(t_token *tok, t_parse_ctx *ctx)
{
    t_cmd   *new;
    t_cmd   *current;

    if (DEBUG_MODE)
    {
        fprintf(stderr, "[LOG] handle_operator_token(): ENTRY\n");
        fprintf(stderr, "[LOG] Token type = %d\n", tok->type);
        fprintf(stderr, "[LOG] ctx->curr before finalize = %p\n", (void *)ctx->curr);
    }
    if (tok->type == TOKEN_PIPE || tok->type == TOKEN_AND || tok->type == TOKEN_OR)
    {

        current = ctx->curr;
        if (!current)
        {
            if (DEBUG_MODE)
                fprintf(stderr, "[LOG][FATAL] current is NULL before finalize_cmd_args() !\n");
        }

        if (DEBUG_MODE)
            fprintf(stderr, "[LOG] Calling finalize_cmd_args() with ctx->curr = %p\n", (void *)ctx->curr);
        finalize_cmd_args(ctx);
        if (DEBUG_MODE)
            fprintf(stderr, "[LOG] Returned from finalize_cmd_args()\n");

        if (!current)
        {
            if (DEBUG_MODE)
                fprintf(stderr, "[LOG][FATAL] current is STILL NULL after finalize! Cannot assign next_type!\n");
            abort();
        }

        if (DEBUG_MODE)
        {
            fprintf(stderr, "[LOG] handle_operator_token(): current = %p\n", (void *)current);
            fprintf(stderr, "[LOG] handle_operator_token(): tok->type = %d\n", tok->type);
            fprintf(stderr, "[LOG] Setting current->next_type = %d\n", tok->type);  
        }
        current->next_type = tok->type; // Assigns the operator type to the *previous* command.

        new = setup_new_cmd(); // Allocate the NEW command structure.
        if (DEBUG_MODE)
            fprintf(stderr, "[LOG] New cmd allocated at %p\n", (void *)new);

        current->next = new; // Link the previous command to the new one.
        new->prev = current;
        new->prev_type = tok->type;

        ctx->curr = new; // Update ctx->curr to point to the new command.
        if (DEBUG_MODE)
            fprintf(stderr, "[LOG] ctx->curr updated to %p\n", (void *)ctx->curr);

        // REMOVE THESE LINES -- finalize_cmd_args already handled them!
        // if (ctx->args)
        //     free_tmp_args(ctx->args, ctx->arg_i);
        // free(ctx->args);
        // free(ctx->quote_chars);

        // RE-ALLOCATE the temporary buffers for the *next* command
        // that will be parsed after this operator.
        ctx->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
        ctx->quote_chars = malloc(sizeof(t_quote_state) * MAX_CMD_ARGS);
        if (!ctx->args || !ctx->quote_chars)
            exit_error("malloc failed");
        // It's good practice to clear the newly allocated memory.
        ft_memset(ctx->args, 0, sizeof(char *) * MAX_CMD_ARGS);
        ft_memset(ctx->quote_chars, 0, sizeof(t_quote_state) * MAX_CMD_ARGS);

        ctx->arg_i = 0; // Reset indexes for the new command.
        ctx->quote_i = 0;
        if (DEBUG_MODE)
            fprintf(stderr, "[LOG] handle_operator_token(): END\n");
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

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] init_parse_context(): allocating ctx->args & ctx->quote_chars\n");
	ctx->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
	ctx->quote_chars = malloc(sizeof(t_quote_state) * MAX_CMD_ARGS);
	if (!ctx->args || !ctx->quote_chars)
		exit_error("malloc args or quote_chars failed");

	ft_memset(ctx->args, 0, sizeof(char *) * MAX_CMD_ARGS);
	ft_memset(ctx->quote_chars, 0, sizeof(t_quote_state) * MAX_CMD_ARGS);
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] init_parse_context(): done. ctx->curr = %p, ctx->head = %p\n", (void *)ctx->curr, (void *)ctx->head);
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
	t_parse_ctx	ctx;

	init_parse_context(&ctx, tok, shell);
	process_token_loop(&ctx, shell, multi_data);

	if (ctx.curr && ctx.arg_i > 0)
		finalize_cmd_args(&ctx);

	if (ctx.args)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Freeing ctx.args content\n");
		free_tmp_args(ctx.args, ctx.arg_i);
		free(ctx.args);
		ctx.args = NULL;
	}

	if (ctx.quote_chars)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Freeing ctx.quote_chars global %p\n", ctx.quote_chars);
		free(ctx.quote_chars);
		ctx.quote_chars = NULL;
	}

	ctx.arg_i = 0;
	ctx.quote_i = 0;
	return (ctx.head);
}

