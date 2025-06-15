/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:33:30 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 22:55:42 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Allocates and initializes a new command structure.
 *
 * Sets default values for arguments, input/output file descriptors,
 * and pointers.
 * @return Pointer to the newly allocated t_cmd structure.
 */
t_cmd	*setup_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit_error("malloc failed");
	cmd->args = NULL;
	cmd->cmd_path = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->prev_type = TOKEN_NONE;
	cmd->next_type = TOKEN_NONE;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	cmd->quote_chars = NULL;
	return (cmd);
}

/**
 * @brief Creates a new command group for a pipeline or subshell.
 *
 * @param cmds Pointer to the first command in the group.
 * @return Pointer to the newly allocated t_group structure,
 * or NULL if cmds is NULL.
 */

t_group	*create_group(t_cmd *cmds)
{
	t_group	*group;

	if (!cmds)
		return (NULL);
	group = malloc(sizeof(t_group));
	if (!group)
		exit_error("malloc group_commands");
	group->cmds = cmds;
	group->next = NULL;
	return (group);
}

/**
 * @brief Adds a group of pipelined commands to the group list.
 *
 * This function collects consecutive commands connected by pipes into a group,
 * creates a t_group node, and appends it to the linked list.
 *
 * @param cmds     Pointer to the first command of the pipeline.
 * @param head     Pointer to the head of the group list.
 * @param curr_group Pointer to the current group in the list (will be updated).
 * @return Pointer to the next command after the pipeline (may be NULL).
 */
static t_cmd	*group_add_pipeline(t_cmd *cmds, t_group **head,
	t_group **curr_group)
{
	t_group	*new_group;
	t_cmd	*next;

	new_group = create_group(cmds);
	if (!new_group)
		return (NULL);
	while (cmds->next && cmds->next_type == TOKEN_PIPE)
		cmds = cmds->next;
	new_group->next_op = cmds->next_type;
	next = cmds->next;
	cmds->next = NULL;
	if (!*head)
		*head = new_group;
	else
		(*curr_group)->next = new_group;
	*curr_group = new_group;
	return (next);
}

/**
 * @brief Groups commands into pipelines and subshell groups for execution.
 *
 * Iterates through the command list, grouping piped commands and
 * subshells into t_group structures.
 *
 * @param cmds Linked list of parsed commands.
 * @return Head of the new linked list of command groups.
 */
t_group	*group_commands(t_cmd *cmds)
{
	t_group	*head;
	t_group	*curr_group;

	head = NULL;
	curr_group = NULL;
	while (cmds)
	{
		if (cmds->cmd_path && ft_strncmp(cmds->cmd_path,
				"__SUBSHELL_", 11) == 0)
			cmds = group_add_subshell(cmds, &head, &curr_group);
		else
			cmds = group_add_pipeline(cmds, &head, &curr_group);
	}
	return (head);
}
