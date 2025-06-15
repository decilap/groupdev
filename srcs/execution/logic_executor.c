/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 00:35:04 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Expand all arguments for one group.
 *
 * @param g The group to expand.
 * @param shell The shell context.
 */
static void	expand_one_group(t_group *g, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = g->cmds;
	while (cmd)
	{
		expand_cmd_args(cmd, shell);
		cmd = cmd->next;
	}
}

/**
 * @brief Skip over groups according to logical operators and status.
 *
 * @param g Current group pointer (passed by reference).
 * @param status Last executed command status.
 */
static void	skip_logical_groups(t_group **g, int status)
{
	(*g) = (*g)->next;
	while (*g)
	{
		if (((*g)->next_op == TOKEN_AND && status != 0)
			|| ((*g)->next_op == TOKEN_OR && status == 0))
			(*g) = (*g)->next;
		else
			break ;
	}
	if (*g)
		(*g) = (*g)->next;
}

/**
 * @brief Entry point for executing a command list with logical operators.
 *
 * Groups commands by logical operators, executes them, sets exit status,
 * and frees the group structures.
 *
 * @param groups The command list to execute.
 * @param shell The shell context.
 * @return The final exit status.
 */
int	execute_logical_groups(t_group *groups, t_shell *shell)
{
	t_group	*g;
	t_cmd	*pipeline;
	int		status;

	status = 0;
	g = groups;
	while (g)
	{
		expand_one_group(g, shell);
		pipeline = g->cmds;
		status = execute_pipeline_group(&pipeline, shell);
		if ((g->next_op == TOKEN_AND && status != 0)
			|| (g->next_op == TOKEN_OR && status == 0))
		{
			skip_logical_groups(&g, status);
			continue ;
		}
		else
			g = g->next;
	}
	return (status);
}
