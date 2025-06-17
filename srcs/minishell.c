/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:21:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/17 13:29:27 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Handles word tokens (command names, arguments) in the lexer.
 *
 * Extracts a word token (respecting quoted substrings), determines if it's a 
 * subshell,
 * creates the appropriate token, and sets the quoted flag.
 *
 * @param line   The input line being lexed.
 * @param i      Pointer to the current position in the line (will be advanced).
 * @param tokens The token list to append to.
 */
// static void	replace_nbsp_inplace(char *s)
// {
// 	int	i;
// 	i = 0;
// 	while (s[i])
// 	{
// 		if ((unsigned char)s[i] == 0xC2 && (unsigned char)s[i + 1] == 0xA0)
// 		{
// 			s[i] = ' ';
// 			i++;
// 			while (s[i])
// 			{
// 				s[i] = s[i + 1];
// 				i++;
// 			}
// 			i = 0;
// 		}
// 		else
// 			i++;
// 	}
// }

void	clean_exit(t_shell *shell)
{
	if (!shell)
		exit(1);
	if (!save_to_file(shell, &shell->history))
		perror(RED "minishell: Error in save_to_file for history" RESET);
	if (shell->history)
		free_history(&shell->history);
	rl_clear_history();
	if (shell->cmds)
		free_cmds(shell->cmds);
	if (shell->env)
		ft_free_arr(shell->env);
	exit(shell->exit_status);
}

void	ft_parsing_and_execute(t_shell *shell, char *result, t_token *tok)
{
	t_cmd	*cmd;

	cmd = parse_input(result, shell, tok);
	if (!cmd)
		return ;
	execute_with_logical(cmd, shell);
	clear_subshell_table();
}

bool	ft_brain_of_minishell(t_shell *shell, char *line)
{
	char	**lines;
	char	*cmd_line;
	int		i;
	t_token	*tok;

	i = 0;
	tok = NULL;
	lines = ft_split(line, '\n');
	if (!lines)
		return (ft_error_brain(shell), false);
	while (lines[i])
	{
		cmd_line = lines[i];
		if (!cmd_line[0] && i++)
			continue ;
		ft_history_loop(shell, lines[i]);
		if (!ft_loop(shell, i + lines, &i, &tok))
			return (ft_free_arr(lines), false);
		i++;
		while (*cmd_line && ft_isspace(*cmd_line))
			cmd_line++;
		ft_parsing_and_execute(shell, cmd_line, tok);
		free_tokens(tok);
	}
	return (ft_free_arr(lines), true);
}

static void	prompt_loop(t_shell *shell)
{
	char	*line;

	if (!initialize_history(shell, &shell->history))
		perror(RED "minishell: Error in initialize_history" RESET);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			return (clean_exit(shell));
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
			return (free(line));
		free(line);
	}
	return ;
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
	perror(RED "\nminishell: Error in prompt_loop" RESET);
	clean_exit(&shell);
	return (404);
}
