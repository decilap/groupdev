/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <ddecilap@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/21 12:42:18 by ddecilap         ###   ########.fr       */
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
	if (count > MAX_PIPE_CMDS)
	{
		ft_putstr_fd("minishell: too many processes in pipeline\
			\n", STDERR_FILENO);
		return (1);
	}
	final_status = exec_pipeline_loop(cmd, shell, pids);
	*cmd_ptr = NULL;
	shell->exit_status = final_status;
	return (final_status);
}
