/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:47:00 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/20 20:38:27 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Allocates memory for command arguments and quote characters.
 *
 * Allocates memory for both the args array and the quote_chars array
 * based on the total argument count.
 *
 * @param cmd The command structure where allocations are stored.
 * @param count Number of arguments to allocate for.
 */
static void	allocate_cmd_args(t_cmd *cmd, int count)
{
	cmd->args = malloc(sizeof(char *) * (count + 1));
	if (!cmd->args)
		exit_error("malloc failed (args)");
	cmd->quote_chars = malloc(sizeof(t_quote_state) * (count + 1));
	if (!cmd->quote_chars)
	{
		free(cmd->args);
		exit_error("malloc failed (quote_chars)");
	}
}

/**
 * @brief Fills the command argument and quote arrays by duplicating input.
 *
 * Copies all arguments and their quote information safely into
 * the command structure.
 * Handles malloc failure safely with partial cleanup.
 *
 * @param cmd The command structure being filled.
 * @param args Input arguments to copy.
 * @param quote_chars Input quote state to copy.
 * @param count Total number of arguments.
 */
static void	fill_cmd_args(t_cmd *cmd, char **args,
	t_quote_state *quote_chars, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		cmd->args[i] = ft_strdup(args[i]);
		if (!cmd->args[i])
		{
			free_partial_args(cmd->args, i);
			free(cmd->args);
			free(cmd->quote_chars);
			exit_error("strdup failed in fill_cmd_args");
			return ;
		}
		cmd->quote_chars[i] = quote_chars[i];
		i++;
	}
	cmd->args[i] = NULL;
	cmd->quote_chars[i] = '\0';
}

/**
 * @brief Expands variables and resolves the command path.
 *
 * Handles variable expansion on the first argument, then attempts
 * to resolve the binary path using the PATH environment variable.
 *
 * @param cmd The command structure.
 * @param shell The shell structure (for env and exit code).
 * @param err_code Error code pointer to store resolution status.
 * @return The expanded token (must be freed after use).
 */
t_token	*expand_and_resolve_path(t_cmd *cmd, t_shell *shell, int *err_code)
{
	t_token	*expanded;

	*err_code = 0;
	if (!cmd->args[0])
	{
		cmd->cmd_path = NULL;
		return (NULL);
	}
	expanded = expand_variables(cmd->args[0], shell, 1, Q_NONE);
	if (expanded && expanded->value && expanded->value[0] != '\0')
		cmd->cmd_path = get_cmd_path(expanded->value, shell->env, err_code);
	else
		cmd->cmd_path = NULL;
	return (expanded);
}

/**
 * @brief Handles error messages if command path was not found.
 *
 * @param cmd The command structure.
 * @param expanded The expanded token (used to check for subshell).
 * @param err_code The error code returned by path resolution.
 * @param shell The shell structure to update exit_status.
 */
void	handle_path_errors(t_cmd *cmd, t_token *expanded, int err_code,
	t_shell *shell)
{
	if (!cmd->cmd_path
		&& (!expanded || ft_strncmp(expanded->value, "__SUBSHELL_", 11) != 0)
		&& !is_builtin(cmd))
	{
		if (err_code == 1)
		{
			ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
			if (expanded && expanded->value)
				ft_putstr_fd(expanded->value, STDERR_FILENO);
			ft_putendl_fd(": No such file or directory" RESET, STDERR_FILENO);
		}
		else if (err_code == 2)
		{
			ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
			if (expanded && expanded->value)
				ft_putstr_fd(expanded->value, STDERR_FILENO);
			ft_putendl_fd(": command not found" RESET, STDERR_FILENO);
		}
		shell->exit_status = 127;
	}
}

/**
 * @brief Finalizes the arguments and resolves the command
 * path for the current command.
 *
 * This function:
 *  - Allocates memory for the current command's arguments and quote characters,
 *  - Copies the temporary arguments and quote states from the
 * parsing context into the command,
 *  - Expands variables and resolves the command's executable path.
 *
 * All memory allocation failures are handled internally in the called functions.
 *
 * @param ctx The parsing context containing temporary argument data.
 */
void	finalize_cmd_args(t_parse_ctx *ctx)
{
	if (!ctx->curr)
	{
		ctx->curr = setup_new_cmd();
		ctx->head = ctx->curr;
	}
	if (ctx->curr->args)
	{
		free_tmp_args(ctx->curr->args, -1);
		free(ctx->curr->args);
	}
	if (ctx->curr->quote_chars)
		free(ctx->curr->quote_chars);
	if (ctx->curr->cmd_path)
	{
		free(ctx->curr->cmd_path);
		ctx->curr->cmd_path = NULL;
	}
	ctx->curr->args = NULL;
	ctx->curr->quote_chars = NULL;
	allocate_cmd_args(ctx->curr, ctx->arg_i);
	fill_cmd_args(ctx->curr, ctx->args, ctx->quote_chars, ctx->arg_i);
	resolve_cmd_path(ctx->curr, ctx->shell);
}
