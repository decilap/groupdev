/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:50:50 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/11 14:11:51 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

void	ft_init_shell_struct(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp);
	shell->cmds = NULL;
	shell->locals = NULL;
	shell->history = NULL;
	shell->hist_len = 0;
	shell->histfile_len = 0;
	shell->exit_status = 0;
	shell->syntax_error_printed = 0;
}

void	ft_init_shlvl_env(t_shell *shell)
{
	int		shlvl;
	char	*shlvl_str;
	char	*new_shlvl;

	shlvl = 0;
	shlvl_str = get_env_value("SHLVL", shell->env);
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	shlvl += 1;
	new_shlvl = ft_itoa(shlvl);
	set_env_value("SHLVL", new_shlvl, shell);
	free(new_shlvl);
}

void	ft_init_history_env(t_shell *shell)
{
	char	*home;
	char	*home_env;

	home_env = get_env_value("HOME", shell->env);
	if (home_env)
		home = ft_strjoin_3(home_env, "/", FILE_HIST);
	if (!home_env)
		home = TMP_FILE_HIST;
	set_env_value("HISTFILE", home, shell);
	set_env_value("HISTTRUNC", "0", shell);
	set_env_value("HISTSIZE", "500", shell);
	set_env_value("HISTFILESIZE", "500", shell);
	if (home_env)
		free(home);
}
