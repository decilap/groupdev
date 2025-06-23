/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/23 13:47:01 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Loops over commands in a pipeline, forking and executing each.
 *
 * Prepares the pipeline context, runs each step, and waits for all children.
 *
 * @param cmd   The command chain (linked list) for this pipeline.
 * @param shell The shell context.
 * @param pids  Array to collect all forked process IDs.
 * @return The final pipeline exit status.
 */
int	exec_pipeline_loop(t_cmd *cmd, t_shell *shell, pid_t *pids)
{
	int				i;
	int				final_status;
	t_pipe_ctx		ctx;
	t_pipeline_ctx	pipeline_ctx;

	i = 0;
	final_status = 0;
	init_pipe_ctx(&ctx);
	pipeline_ctx.pids = pids;
	pipeline_ctx.i = &i;
	pipeline_ctx.pipe_ctx = &ctx;
	pipeline_ctx.depth = 0;
	while (cmd)
	{
		final_status = handle_pipeline_step(cmd, shell, &pipeline_ctx);
		if (final_status == 2)
			break ;
		cmd = cmd->next;
	}
	return (waitpids(pids, i, final_status));
}

/**
 * @brief Entry point for executing a command list with logical operators.
 *
 * Groups commands by logical operators, executes them, sets exit status,
 * and frees the group structures.
 *
 * @param cmd   The command list to execute.
 * @param shell The shell context.
 * @return The final exit status.
 */
int	execute_with_logical(t_cmd *cmd, t_shell *shell)
{
	int		status;
	t_group	*groups;

	groups = group_commands(cmd);
	status = 0;
	if (groups)
	{
		shell->groups = groups;
		status = execute_logical_groups(groups, shell);
	}
	shell->exit_status = status;
	free_groups(groups);
	return (status);
}
