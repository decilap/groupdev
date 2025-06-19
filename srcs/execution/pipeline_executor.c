/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 21:30:43 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static bool	ft_max_pipe(int count, t_shell *shell)
{
	if (count > MAX_PIPE_CMDS)
	{
		shell->exit_status = 2;
		ft_putstr_fd("minishell: too many processes in pipeline\
			\n", STDERR_FILENO);
		return (false);
	}
	return (true);
}

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
int	execute_pipeline_group(t_cmd **cmd_ptr, t_shell *shell)
{
	int		count;
	int		final_status;
	t_cmd	*cmd;
	t_cmd	*cur;
	pid_t	pids[MAX_PIPE_CMDS];

	count = 1;
	cmd = *cmd_ptr;
	cur = cmd;
	while (cur)
	{
		if (cur->next_type == TOKEN_PIPE)
			count++;
		cur = cur->next;
	}
	if (!ft_max_pipe(count, shell))
		return (1);
	final_status = exec_pipeline_loop(cmd, shell, pids);
	*cmd_ptr = NULL;
	if (shell->exit_status != 130)
		shell->exit_status = final_status;
	else
		final_status = 1;
	return (final_status);
}
