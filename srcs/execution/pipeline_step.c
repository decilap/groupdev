/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_step.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 00:50:02 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Handles execution of a single command step in a pipeline.
 *
 * Decides whether to execute a subshell, a builtin, or an external command
 * in a forked process, with correct pipe setup.
 *
 * @param cmd   The command to execute.
 * @param shell The shell context.
 * @param ctx   The pipeline context (pipe file descriptors, pids, etc.).
 * @return 0 on success, or error code (e.g., 2) on error/interrupt.
 */

static bool	should_fork_builtin(t_cmd *cmd, bool in_pipeline)
{
	return (in_pipeline || cmd->fd_in != STDIN_FILENO
		|| cmd->fd_out != STDOUT_FILENO);
}

/**
 * @brief Check if 'export' is called without any argument (display mode).
 * 
 * @param cmd The command structure.
 * @return true if export with no arguments, false otherwise.
 */
static bool	is_export_display_only(t_cmd *cmd)
{
	return (is_export_builtin(cmd) && cmd->args[1] == NULL);
}

/**
 * @brief Handle all parent-executed commands before fork
 * (subshell, export, builtin).
 *
 * @return 1 if command was handled here, 0 otherwise.
 */
static int	handle_pipeline_prechecks(t_cmd *cmd, t_shell *shell,
	t_pipeline_ctx *ctx)
{
	bool	in_pipeline;
	bool	is_export;
	bool	export_display;

	if (DEBUG_MODE)
	{
		fprintf(stderr, "[LOG] handle_pipeline_prechecks(): ENTRY\n");
		fprintf(stderr, "[LOG] cmd = %p, cmd->args[0] = \"%s\"\n", 
			(void *)cmd, cmd->args ? cmd->args[0] : "(null)");
	}
	if (is_subshell(cmd))
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Detected subshell — calling step_subshell()\n");
		return (step_subshell(cmd, shell, ctx));
	}
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Invalid fd_in (%d) or fd_out (%d), skipping command\n",
			cmd->fd_in, cmd->fd_out);
		return (shell->exit_status);
	}

	in_pipeline = (cmd->next_type == TOKEN_PIPE
		|| (cmd->prev && cmd->prev_type == TOKEN_PIPE));
	is_export = is_export_builtin(cmd);
	export_display = is_export_display_only(cmd);

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] in_pipeline = %d, is_export = %d, export_display = %d\n",
		in_pipeline, is_export, export_display);

	if (is_export && !export_display && !in_pipeline)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Executing export with args (no display mode)\n");
		shell->exit_status = execute_builtin(cmd, shell);
		update_ctx_after_cmd(ctx->pipe_ctx);
		return (shell->exit_status);
	}
	if (is_builtin(cmd) && !should_fork_builtin(cmd, in_pipeline))
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] Executing builtin outside of fork\n");
		shell->exit_status = execute_builtin(cmd, shell);
		update_ctx_after_cmd(ctx->pipe_ctx);
		return (shell->exit_status);
	}
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] handle_pipeline_prechecks(): END — need fork\n");
	return (-1);
}


/**
 * @brief Fork and execute command inside child process.
 */
static void	handle_pipeline_fork(t_cmd *cmd, t_shell *shell,
	t_pipeline_ctx *ctx)
{
	struct stat	sb;

	pipe_prepare(ctx->pipe_ctx, cmd);
	ctx->pids[*(ctx->i)] = fork();
	if (ctx->pids[*(ctx->i)] == -1)
	{
		perror("fork failed");
		exit(1);
	}
	if (ctx->pids[*(ctx->i)] == 0)
		child_process(cmd, shell, ctx->pipe_ctx, &sb);
}

/**
 * @brief Execute one step of a pipeline (single command execution).
 */
int	handle_pipeline_step(t_cmd *cmd, t_shell *shell, t_pipeline_ctx *ctx)
{
	int	precheck;

	if (DEBUG_MODE)
	{
		if (cmd && cmd->args && cmd->args[0])
			fprintf(stderr, "[LOG] Command = \"%s\"\n", cmd->args[0]);
		else
			fprintf(stderr, "[LOG] Command is NULL or missing args\n");
		fprintf(stderr, "[LOG] handle_pipeline_step(): ENTRY — cmd = %p\n", (void *)cmd);
	}
/* ==> */pipe_prepare(ctx->pipe_ctx, cmd);
	precheck = handle_pipeline_prechecks(cmd, shell, ctx);
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] handle_pipeline_step(): precheck = %d\n", precheck);

	if (precheck != -1)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] handle_pipeline_step(): skipping fork — handled by precheck\n");
		return (precheck);
	}
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] handle_pipeline_step(): calling handle_pipeline_fork()\n");
	handle_pipeline_fork(cmd, shell, ctx);

	(*(ctx->i))++;
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] handle_pipeline_step(): incremented ctx->i to %d\n", *(ctx->i));

	update_ctx_after_cmd(ctx->pipe_ctx);
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] handle_pipeline_step(): EXIT\n");
	return (0);
}
