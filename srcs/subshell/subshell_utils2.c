/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:44:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 21:14:57 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Checks if a command represents a subshell.
 *
 * Identifies a subshell by testing if the first argument matches the
 * internal subshell tag (e.g., "__SUBSHELL_N__").
 *
 * @param cmd The command to check.
 * @return 1 if the command is a subshell; 0 otherwise.
 */
int	is_subshell(t_cmd *cmd)
{
	if (!cmd->args)
		return (0);
	if (!cmd->args[0])
		return (0);
	if (ft_strncmp(cmd->args[0], "__SUBSHELL_", 11) == 0)
		return (1);
	return (0);
}

/**
 * @brief Cleans up and exits from a subshell.
 *
 * Frees the shell environment, restores the previous environment backup,
 * and exits with the given status code.
 *
 * @param shell      The main shell context.
 * @param env_backup The environment backup to restore.
 * @param exit_code  The exit status to use.
 */
void	subshell_clean_exit(t_shell *shell, char **env_backup, int exit_code)
{
	clear_subshell_table();
	if (shell->cmds)
		free_cmds(shell->cmds);
	shell->cmds = NULL;
	ft_free_arr(shell->env);
	shell->env = env_backup;
	exit(exit_code);
}
