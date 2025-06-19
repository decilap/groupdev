/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 17:58:31 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Extracts a redirection operator from the input line.
 *
 * Supports both single and double character redirections: >, >>, <, <<.
 * Advances the parsing index accordingly.
 *
 * @param line The input command line.
 * @param i Pointer to the current parsing index (will be updated).
 * @return Newly allocated operator string, or NULL on allocation failure.
 */
static char	*extract_redir_op(const char *line, int *i)
{
	int		start;
	char	*op;

	start = (*i)++;
	if (line[*i] == line[start])
		(*i)++;
	op = ft_substr(line, start, *i - start);
	if (!op)
		perror("lexer: ft_substr failed");
	return (op);
}

/**
 * @brief Handles redirection tokens during lexing.
 *
 * Extracts the operator (>, >>, <, <<), determines its token type,
 * creates a new token, and appends it to the token list.
 *
 * All memory allocations are checked, and errors are printed to stderr.
 *
 * @param line The input command line.
 * @param i Pointer to the current parsing index (will be updated).
 * @param tokens Pointer to the token list to append to.
 */
void	lexer_handle_redir(t_lexer_ctx *ctx)
{
	int				type;
	char			*op;
	t_token_data	data;
	t_token			*tok;

	op = extract_redir_op(ctx->line, &ctx->i);
	if (!op)
		return ;
	type = get_token_type(op);
	data = (t_token_data){op, type, 0, Q_NONE};
	tok = add_token(&ctx->tokens, data);
	free(op); // libération systématique, même si add_token réussit

	if (!tok)
		perror("lexer: add_token failed");
}


