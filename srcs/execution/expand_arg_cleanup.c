/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg_cleanup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 00:51:21 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Handles empty quoted argument cases.
 *
 * If argument is quoted but empty and next argument is non-empty, skip merge.
 *
 * @param cmd The command structure.
 * @param i Current argument index.
 * @return 1 if handled, 0 otherwise.
 */
static int	handle_empty_quoted(t_cmd *cmd, int i)
{
	if (cmd->args[i][0] == '\0' && cmd->quote_chars[i] != Q_NONE &&
		cmd->args[i + 1] && cmd->args[i + 1][0] != '\0')
		return (1);
	return (0);
}

/**
 * @brief Merges unquoted empty arguments with the next argument.
 *
 * @param cmd The command structure.
 * @param i Current argument index.
 * @return 1 if merged, 0 otherwise.
 */
static int	merge_empty_unquoted(t_cmd *cmd, int i)
{
	char	*tmp;
	int		j;

	if (cmd->args[i][0] == '\0' && cmd->quote_chars[i] == Q_NONE
		&& cmd->args[i + 1])
	{
		tmp = ft_strjoin(cmd->args[i], cmd->args[i + 1]);
		free(cmd->args[i]);
		free(cmd->args[i + 1]);
		cmd->args[i] = tmp;
		j = i + 1;
		while (cmd->args[j])
		{
			cmd->args[j] = cmd->args[j + 1];
			cmd->quote_chars[j] = cmd->quote_chars[j + 1];
			j++;
		}
		return (1);
	}
	return (0);
}

/**
 * @brief Removes fully empty unquoted arguments.
 *
 * @param cmd The command structure.
 * @param i Current argument index.
 * @return 1 if removed, 0 otherwise.
 */
static int	remove_empty_unquoted(t_cmd *cmd, int i)
{
	int	j;

	if (cmd->args[i][0] == '\0' && cmd->quote_chars[i] == Q_NONE)
	{
		free(cmd->args[i]);
		j = i;
		while (cmd->args[j])
		{
			cmd->args[j] = cmd->args[j + 1];
			cmd->quote_chars[j] = cmd->quote_chars[j + 1];
			j++;
		}
		return (1);
	}
	return (0);
}

/**
 * @brief Expands all arguments of a command.
 *
 * Applies variable expansion, argument merge, and cleanup logic.
 *
 * @param cmd The command structure.
 * @param shell The shell context.
 */
void	expand_cmd_args(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (!cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		expand_single_arg(cmd, shell, i);
		if (handle_empty_quoted(cmd, i))
		{
			i++;
			continue ;
		}
		if (merge_empty_unquoted(cmd, i))
			continue ;
		if (remove_empty_unquoted(cmd, i))
			continue ;
		i++;
	}
}
