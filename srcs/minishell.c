/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:21:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 02:22:02 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

void	clean_exit(t_shell *shell, int use_history)
{
	if (!shell)
		exit(1);
	if (!save_to_file(shell, &shell->history))
		perror(RED "minishell: Error in save_to_file for history" RESET);
	if (shell->history)
		free_history(&shell->history);
	if (shell->cmds)
		free_cmds(shell->cmds);
	if (shell->env)
		ft_free_arr(shell->env);
	if (use_history)
		rl_clear_history();
	exit(shell->exit_status);
}

static bool	ft_brain_of_minishell(t_shell *shell, char *line)
{
	int	ret_multi;

	ret_multi = has_complete_multiline_heredoc(line);
	if (ret_multi < 0)
	{
		shell->exit_status = 1;
		perror(RED "minishell: Error in has_complete_multiline_heredoc" RESET);
		return (free(line), false);
	}
	if (ret_multi > 0 && !ft_loop_with_herdoc(shell, line))
	{
		shell->exit_status = 1;
		(RED "minishell: Error in ft_loop_with_herdoc" RESET);
		return (free(line), false);
	}
	if (ret_multi == 0 && !ft_loop(shell, line))
	{
		shell->exit_status = 1;
		perror(RED "minishell: Error in ft_loop" RESET);
		return (free(line), false);
	}
	return (true);
}

//tester multi herdoc echo lol && cat << EOF && cat << EOF
static bool	prompt_loop(t_shell *shell)
{
	char	*line;

	if (!initialize_history(shell, &shell->history))
		perror(RED "minishell: Error in initialize_history" RESET);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			clean_exit(shell, 1);
		if (g_signal == SIGINT)
		{
			shell->exit_status = 130;
			g_signal = 0;
		}
		if (!line[0])
		{
			free(line);
			continue ;
		}
		if (!ft_brain_of_minishell(shell, line))
			return (false);
		free(line);
	}
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_init_shell_struct(&shell, envp);
	ft_init_shlvl_env(&shell);
	ft_init_history_env(&shell);
	init_signals();
	prompt_loop(&shell);
	clean_exit(&shell, 1);
	return (shell.exit_status);
}
