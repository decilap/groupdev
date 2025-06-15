/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:06:52 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 17:01:51 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Frees the internal resources of a command node (t_cmd) 
 * without freeing the struct itself.
 *
 * This function safely releases:
 * - the argument array (args),
 * - the command path (cmd_path),
 * - the quote characters array (quote_chars).
 *
 * It does not free the t_cmd struct itself — that is handled separately.
 *
 * @param cmd The command whose internal resources should be freed.
 */
static void	free_cmd_resources(t_cmd *cmd)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
		cmd->args = NULL;
	}
	if (cmd->cmd_path)
	{
		free(cmd->cmd_path);
		cmd->cmd_path = NULL;
	}
	if (cmd->quote_chars)
	{
		free(cmd->quote_chars);
		cmd->quote_chars = NULL;
	}
}

/**
 * @brief Completely frees the entire linked list of command nodes (t_cmd).
 *
 * For each command node:
 * - Internal resources are freed via free_cmd_resources(),
 * - Input/output file descriptors are closed if necessary,
 * - The t_cmd structure itself is freed.
 *
 * After calling this function, all memory and file descriptors
 * used by the command chain are properly released.
 *
 * @param cmd Pointer to the head of the command list to free.
 */
void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_cmd_resources(cmd);
		if (cmd->fd_in != STDIN_FILENO && cmd->fd_in != -1)
			close(cmd->fd_in);
		if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out != -1)
			close(cmd->fd_out);
		free(cmd);
		cmd = tmp;
	}
}

/**
 * @brief Frees the entire linked list of command groups (t_group).
 *
 * Each group contains a chain of commands. This function first frees
 * the associated command chain via free_cmds(), then frees the group itself.
 *
 * After calling this function, all memory used by the group 
 * chain is properly released.
 *
 * @param group Pointer to the head of the group list to free.
 */

void	free_groups(t_group *group)
{
	t_group	*tmp;

	while (group)
	{
		tmp = group->next;
		if (group->cmds)
		{
			free_cmds(group->cmds);
			group->cmds = NULL;
		}
		free(group);
		group = tmp;
	}
}
