/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:47:00 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/17 13:42:41 by ryoussfi         ###   ########.fr       */
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
/* static ==> */void	allocate_cmd_args(t_cmd *cmd, int count)
{
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] allocate_cmd_args(): cmd = %p, count = %d\n", (void *)cmd, count);

	if (!cmd)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[FATAL] allocate_cmd_args(): cmd is NULL !\n");
		abort();
	}

	cmd->args = malloc(sizeof(char *) * (count + 1));
	if (!cmd->args)
		exit_error("malloc( failed (args)");
	cmd->quote_chars = malloc(sizeof(t_quote_state) * (count + 1));
	if (!cmd->quote_chars)
	{
		free(cmd->args);
		exit_error("malloc( failed (quote_chars)");
	}

	

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] allocate_cmd_args(): allocations OK -> args=%p, quote_chars=%p\n",
		(void *)cmd->args, (void *)cmd->quote_chars);
}


/**
 * @brief Fills the command argument and quote arrays by duplicating input.
 *
 * Copies all arguments and their quote information safely into
 * the command structure.
 * Handles malloc( failure safely with partial cleanup.
 *
 * @param cmd The command structure being filled.
 * @param args Input arguments to copy.
 * @param quote_chars Input quote state to copy.
 * @param count Total number of arguments.
 */
/* static => */void	fill_cmd_args(t_cmd *cmd, char **args,
	t_quote_state *quote_chars, int count)
{
	int	i;

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] fill_cmd_args(): ENTRY — count = %d\n", count);
	i = 0;
	while (i < count)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] fill_cmd_args(): duplicating args[%d] = \"%s\"\n", i, args[i]);
		cmd->args[i] = ft_strdup(args[i]);
		if (!cmd->args[i])
		{
			if (DEBUG_MODE)
				fprintf(stderr, "[ERROR] fill_cmd_args(): ft_strdup failed at i = %d\n", i);
			free_partial_args(cmd->args, i);
			free(cmd->args);
			free(cmd->quote_chars);
			exit_error("strdup failed in fill_cmd_args");
		}
		cmd->quote_chars[i] = quote_chars[i];
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] fill_cmd_args(): quote_chars[%d] = %d\n", i, quote_chars[i]);
		i++;
	}
	cmd->args[i] = NULL;
	cmd->quote_chars[i] = '\0';
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] fill_cmd_args(): EXIT\n");
}

bool	is_subshell_marker(const char *s)
{
	return (s && ft_strncmp(s, "__SUBSHELL_", 11) == 0);
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

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] expand_and_resolve_path(): ENTRY — cmd = %p\n", (void *)cmd);
	*err_code = 0;

	if (!cmd->args[0])
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] expand_and_resolve_path(): cmd->args[0] is NULL — skipping\n");
		cmd->cmd_path = NULL;
		return (NULL);
	}

if (is_subshell_marker(cmd->args[0]))
{
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] expand_and_resolve_path(): Detected subshell marker \"%s\" — skipping path resolution\n", cmd->args[0]);

	// ⛔️ Supprime ça :
	// cmd->cmd_path = ft_strdup(cmd->args[0]);

	cmd->cmd_path = NULL; // ✅ Très important : on n'exécute pas, on passe au step_subshell()
	return (NULL);
}

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] expand_and_resolve_path(): expanding cmd->args[0] = \"%s\"\n", cmd->args[0]);
	expanded = expand_variables(cmd->args[0], shell, 1, Q_NONE);

	if (DEBUG_MODE)
	{
		if (expanded)
			fprintf(stderr, "[LOG] expand_and_resolve_path(): expanded->value = \"%s\"\n", expanded->value);
		else
			fprintf(stderr, "[LOG] expand_and_resolve_path(): expanded is NULL\n");
	}

	if (expanded && expanded->value && expanded->value[0] != '\0')
	{
		cmd->cmd_path = get_cmd_path(expanded->value, shell->env, err_code);
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] expand_and_resolve_path(): get_cmd_path() returned: %s, err_code = %d\n",
				cmd->cmd_path ? cmd->cmd_path : "NULL", *err_code);
	}
	else
	{
		cmd->cmd_path = NULL;
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] expand_and_resolve_path(): expanded is empty — cmd_path set to NULL\n");
	}

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] expand_and_resolve_path(): END — returning expanded = %p\n", (void *)expanded);
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
			ft_putstr_fd(expanded->value, STDERR_FILENO);
			ft_putendl_fd(": No such file or directory" RESET, STDERR_FILENO);
		}
		else if (err_code == 2)
		{
			ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
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
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG][FATAL] finalize_cmd_args(): ctx->curr is NULL — calling setup_new_cmd()\n");
		ctx->curr = setup_new_cmd();
		ctx->head = ctx->curr;
	}
	else if (DEBUG_MODE)
		fprintf(stderr, "[LOG] finalize_cmd_args(): ctx->curr is valid at %p\n", (void *)ctx->curr);

	// ❌ Si on n'a rien à traiter, on ne fait rien
	if (ctx->arg_i == 0)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] finalize_cmd_args(): no args to finalize, skipping\n");
		return ;
	}

	// 🔥 libère anciens contenus
	if (ctx->curr->args)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Freeing args at %p\n", (void *)ctx->curr->args);
		free_tmp_args(ctx->curr->args, -1);
		free(ctx->curr->args);
		ctx->curr->args = NULL;
	}
	if (ctx->curr->quote_chars)
	{
		free(ctx->curr->quote_chars);
		ctx->curr->quote_chars = NULL;
	}
	if (ctx->curr->cmd_path)
	{
		free(ctx->curr->cmd_path);
		ctx->curr->cmd_path = NULL;
	}

	// ✅ Remplissage
	allocate_cmd_args(ctx->curr, ctx->arg_i);
	fill_cmd_args(ctx->curr, ctx->args, ctx->quote_chars, ctx->arg_i);
	resolve_cmd_path(ctx->curr, ctx->shell);

	// 🧹 Libération temporaire
	if (ctx->args)
	{
		free_tmp_args(ctx->args, ctx->arg_i);
		free(ctx->args);
		ctx->args = NULL;
	}
	if (ctx->quote_chars)
	{
		free(ctx->quote_chars);
		ctx->quote_chars = NULL;
	}

	ctx->arg_i = 0;
	ctx->quote_i = 0;
}
