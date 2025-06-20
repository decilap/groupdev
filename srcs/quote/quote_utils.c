/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:02:20 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 12:05:06 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

void	reset_args_and_quotes(t_parse_ctx *ctx)
{
	if (ctx->args)
		free_tmp_args(ctx->args, ctx->arg_i);
	free(ctx->args);
	free(ctx->quote_chars);
	ctx->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
	ctx->quote_chars = malloc(sizeof(t_quote_state) * MAX_CMD_ARGS);
	if (!ctx->args || !ctx->quote_chars)
		exit_error("malloc failed");
	ft_memset(ctx->args, 0, sizeof(char *) * MAX_CMD_ARGS);
	ft_memset(ctx->quote_chars, 0, sizeof(t_quote_state) * MAX_CMD_ARGS);
	ctx->arg_i = 0;
	ctx->quote_i = 0;
}
