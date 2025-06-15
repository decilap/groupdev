/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 01:17:43 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Handle expansion of special variable `$?` (exit status).
 * 
 * Converts the shell's exit status into a string, appends it to the result,
 * and advances the parsing index.
 *
 * @param ctx The expander context.
 * @param j   The index position after the '$' character.
 * @return 1 on success, 0 on failure (allocation error).
 */

static int	handle_exit_status(t_expander *ctx, size_t j)
{
	char	*value;
	char	*new_res;

	value = ft_itoa(ctx->shell->exit_status);
	if (!value)
		return (0);
	new_res = str_append(*(ctx->result), value);
	free(value);
	if (!new_res)
		return (0);
	*(ctx->result) = new_res;
	*(ctx->i) = j + 1;
	return (1);
}

/**
 * @brief Handle expansion of $ followed by digits (e.g. $1, $42).
 * 
 * POSIX allows $digit, but minishell ignores digits and expands to empty string.
 */
static int	handle_digit_reference(t_expander *ctx, size_t *j)
{
	char	*new_res;

	new_res = str_append(*(ctx->result), "");
	if (!new_res)
		return (0);
	*(ctx->result) = new_res;
	while (ft_isdigit(ctx->input[*j]))
		(*j)++;
	*(ctx->i) = *j;
	return (1);
}

/**
 * @brief Handle expansion when the character after $ is invalid
 * (non-alpha, non-underscore).
 * 
 * In this case, we simply treat $ as a normal character and append it.
 */
static int	handle_invalid_identifier(t_expander *ctx)
{
	return (expand_append_char(ctx->result, ctx->input[*(ctx->i)], ctx->i));
}

/**
 * @brief Handle normal variable expansion for valid variable names ($VAR_NAME).
 */
static int	handle_normal_variable(t_expander *ctx, size_t j)
{
	char	*value;
	char	*new_res;
	size_t	next_pos;

	value = get_variable_value(ctx, j, &next_pos);
	if (!value)
		return (0);
	new_res = str_append(*(ctx->result), value);
	free(value);
	if (!new_res)
		return (0);
	*(ctx->result) = new_res;
	*(ctx->i) = next_pos;
	return (1);
}

/**
 * @brief Expand $: main entry point for dollar expansion.
 * 
 * Handles special cases: $?, $digit, invalid identifiers, and normal variables.
 */
int	expand_dollar(t_expander *ctx)
{
	size_t	j;

	j = *(ctx->i) + 1;
	if (ctx->input[j] == '?')
		return (handle_exit_status(ctx, j));
	if (ft_isdigit(ctx->input[j]))
		return (handle_digit_reference(ctx, &j));
	if (!ft_isalpha(ctx->input[j]) && ctx->input[j] != '_')
		return (handle_invalid_identifier(ctx));
	return (handle_normal_variable(ctx, j));
}
