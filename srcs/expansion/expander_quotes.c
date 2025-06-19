/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 01:05:39 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Handles character expansion when not inside quotes.
 *
 * Switches quote state when encountering a single or double quote,
 * processes variable expansion on '$', or simply appends the character.
 *
 * @param ctx The expander context (state, input, result).
 * @return 1 on success, 0 on allocation or expansion failure.
 */
int	expand_none(t_expander *ctx)
{
	char	c;

	c = ctx->input[*(ctx->i)];
	if (c == '\'')
		return (expand_set_state(ctx->state, ctx->i, Q_SINGLE_QUOTE));
	if (c == '"')
		return (expand_set_state(ctx->state, ctx->i, Q_DOUBLE_QUOTE));
	if (c == '$')
		return (expand_dollar(ctx));
	return (expand_append_char(ctx->result, c, ctx->i));
}

/**
 * @brief Handles expansion logic when inside single quotes.
 *
 * Only ends the single quote state when another single quote is found,
 * otherwise appends all characters literally.
 *
 * @param ctx The expander context.
 * @return 1 on success, 0 on allocation failure.
 */
int	expand_single(t_expander *ctx)
{
	char	c;

	c = ctx->input[*(ctx->i)];
	if (c == '\'')
		return (expand_set_state(ctx->state, ctx->i, Q_NONE));
	return (expand_append_char(ctx->result, c, ctx->i));
}

/**
 * @brief Handles expansion logic when inside double quotes.
 *
 * Ends double quote state on '"', expands variables on '$',
 * otherwise appends the character.
 *
 * @param ctx The expander context.
 * @return 1 on success, 0 on allocation or expansion failure.
 */
int	expand_double(t_expander *ctx)
{
	char	c;

	c = ctx->input[*(ctx->i)];
	if (c == '"')
		return (expand_set_state(ctx->state, ctx->i, Q_NONE));
	if (c == '$')
		return (expand_dollar(ctx));
	return (expand_append_char(ctx->result, c, ctx->i));
}

/**
 * @brief Expand heredoc: handles dollar or appends character.
 * 
 * In heredocs, expansion happens if dollar found, otherwise simply appends.
 */
int	expand_heredoc(t_expander *ctx)
{
	char	c;

	c = ctx->input[*(ctx->i)];
	if (c == '$')
		return (expand_dollar(ctx));
	return (expand_append_char(ctx->result, c, ctx->i));
}
