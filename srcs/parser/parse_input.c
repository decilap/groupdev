/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:35:24 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 13:29:11 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Handles the parsing of a subshell token in the token list.
 *
 * If the current token is a TOKEN_SUBSHELL, fills the
 * argument/context structures,
 * allocates new argument arrays if needed, and initializes
 * the current command node.
 *
 * @param ctx   The parsing context containing current command and tokens.
 * @return 1 if a subshell was processed; 0 otherwise.
 */
int	process_token_subshell(t_parse_ctx *ctx)
{
	if (ctx->tok->type != TOKEN_SUBSHELL)
		return (0);

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] process_token_subshell(): token = %s\n", ctx->tok->value);

	ctx->args[ctx->arg_i] = ft_strdup(ctx->tok->value);
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] ft_strdup -> ctx->args[%d] = \"%s\" (%p)\n", ctx->arg_i, ctx->args[ctx->arg_i], (void *)ctx->args[ctx->arg_i]);
	ctx->arg_i++;
	ctx->args[ctx->arg_i] = NULL;
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] Freeing ctx.args content\n");
	finalize_cmd_args(ctx);
	if (DEBUG_MODE)
	{
		for (int i = 0; i < ctx->arg_i; i++)
			if (ctx->args[i])
				fprintf(stderr, "[LOG] Freeing ctx->args[%d]: \"%s\" at %p\n", i, ctx->args[i], (void *)ctx->args[i]);
	
	}
	if (DEBUG_MODE)
	{
		fprintf(stderr, "[LOG] Setting cmd_path on ctx->curr = %p\n", (void *)ctx->curr);
		fprintf(stderr, "[LOG] ft_strdup -> cmd_path = \"%s\" (%p)\n", ctx->curr->cmd_path, (void *)ctx->curr->cmd_path);
	}
	ctx->tok = ctx->tok->next;
	if (ctx->tok && (ctx->tok->type == TOKEN_WORD || ctx->tok->type == TOKEN_SUBSHELL))
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Next token is WORD or SUBSHELL, preparing new cmd\n");
		free_tmp_args(ctx->args, ctx->arg_i);
		free(ctx->args);
		free(ctx->quote_chars);

		ctx->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
		if (!ctx->args)
			exit_error("malloc( args failed");
		ft_memset(ctx->args, 0, sizeof(char *) * MAX_CMD_ARGS);
		ft_memset(ctx->quote_chars, 0, sizeof(t_quote_state) * MAX_CMD_ARGS);
		ctx->arg_i = 0;
		ctx->quote_i = 0;
		ctx->curr = setup_new_cmd();
		if (!ctx->head)
			ctx->head = ctx->curr;
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] setup_new_cmd() returned %p\n", (void *)ctx->curr);
	}
	return (1);
}

/**
 * @brief Validate token list then apply wildcard expansion on eligible tokens.
 *
 * @param tokens The full token list to process.
 * @param shell The shell state.
 * @return Updated token list or NULL on failure.
 */
t_token	*validate_and_expand_wildcards(t_token *tokens, t_shell *shell)
{
	int		res;
	t_token	*cur;

	cur = tokens;
	if (!validate_pipe_logic(tokens, shell)
		|| !validate_redirections(tokens, shell))
		return (NULL);
	while (cur)
	{
		if (cur->type == TOKEN_WORD && cur->quoted == 0
			&& ft_strchr(cur->value, '*') && !ft_strchr(cur->value, '='))
		{
			res = expand_wildcard_for_token(&tokens, cur);
			if (res == -1)
				return (NULL);
			if (res == 1)
			{
				cur = cur->next;
				continue ;
			}
		}
		cur = cur->next;
	}
	return (tokens);
}

/**
 * @brief Replace subshells and handle identifier correction.
 *
 * @param trimmed Trimmed input string.
 * @param line Original input string.
 * @param shell Shell context.
 * @return New string with subshells replaced or NULL if error.
 */
static char	*replace_and_identifier(char *trimmed, char *line, t_shell *shell)
{
	char	*replaced;
	char	*first_word;

	replaced = replace_subshells(line);
	if (!replaced)
	{
		shell->exit_status = 2;
		free(trimmed);
		return (NULL);
	}
	if (!is_valid_identifier(trimmed))
	{
		first_word = extract_first_word(trimmed);
		free(trimmed);
		free(first_word);
	}
	else
		free(trimmed);
	return (replaced);
}

/**
 * @brief Full preprocessing: syntax check, assignment, subshells, identifiers.
 *
 * @param line The raw input command.
 * @param shell Shell context.
 * @return Preprocessed string ready for parsing, or NULL if error.
 */
static char	*preprocess_input(char *line, t_shell *shell)
{
	char	*trimmed;
	char	*replaced;

	trimmed = validate_and_trim(line, shell);
	if (!trimmed)
		return (NULL);
	replaced = replace_and_identifier(trimmed, line, shell);
	return (replaced);
}

/**
 * @brief Parses the command line into a command structure list.
 *
 * Performs pre-checks on the line (syntax, assignment, subshell replacement),
 * and parses the result into a linked list of t_cmd nodes.
 *
 * @param line   The input command line.
 * @param shell  Pointer to the shell structure for context and error reporting.
 * @param multi_data Optional multi-line heredoc data.
 * @return The parsed command list (t_cmd*), or NULL if an error occurred.
 */
t_cmd	*parse_input(char *line, t_shell *shell, t_token *multi_data)
{
	char	*replaced;
	t_cmd	*result;

	replaced = preprocess_input(line, shell);
	if (!replaced)
		return (NULL);
	result = parse_tokens_flow(replaced, shell, multi_data);
	free(replaced);
	return (result);
}
