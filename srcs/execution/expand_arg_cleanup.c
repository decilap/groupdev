/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg_cleanup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 21:12:48 by ryoussfi         ###   ########.fr       */
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
		if (!tmp)
			return (-1);
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
bool	expand_cmd_args(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	ret_m_e_uq;

	if (!cmd || !cmd->args)
		return (false);
	i = 0;
	while (cmd->args[i])
	{
		if (!expand_single_arg(cmd, shell, i))
			return (false);
		if (handle_empty_quoted(cmd, i) && i++)
			continue ;
		ret_m_e_uq = merge_empty_unquoted(cmd, i);
		if (ret_m_e_uq < 0)
			return (shell->exit_status = 2,
				perror(RED "minishell: execution: merge_empty_unquoted" RESET),
				false);
		if (ret_m_e_uq)
			continue ;
		if (remove_empty_unquoted(cmd, i))
			continue ;
		i++;
	}
	return (true);
}
