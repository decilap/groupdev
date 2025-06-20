/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:21:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/20 20:37:46 by ryoussfi         ###   ########.fr       */
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
static void	replace_nbsp_inplace(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if ((unsigned char)s[i] == 0xC2 && (unsigned char)s[i + 1] == 0xA0)
		{
			s[i] = ' ';
			i++;
			while (s[i])
			{
				s[i] = s[i + 1];
				i++;
			}
			i = 0;
		}
		else
			i++;
	}
}

/**
 * @brief Parses a line and executes the resulting command list.
 *
 * Parses the input string into commands, executes them with logical operators,
 * and clears any stored subshells.
 *
 * @param shell Pointer to the main shell structure.
 * @param result The input command line (possibly cleaned).
 * @param tok Optional token pointer (used for multi-line/interactive inputs).
 */
static void	ft_parsing_and_execute(t_shell *shell, char *result, t_token *tok)
{
	t_cmd	*cmd;

	cmd = parse_input(result, shell, tok);
	if (!cmd)
		return ;
	execute_with_logical(cmd, shell);
	clear_subshell_table();
}

/**
 * @brief Core logic of the shell loop — parses and executes all user input.
 *
 * Splits the input line by newlines, processes each command line individually:
 * replaces non-breaking spaces, updates history, and executes parsed input.
 *
 * @param shell Pointer to the shell structure.
 * @param line The full user input line (may contain multiple lines).
 * @return true on success, false on fatal error (e.g., memory allocation).
 */
static bool	ft_brain_of_minishell(t_shell *shell, char *line)
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
		replace_nbsp_inplace(cmd_line);
		if (!cmd_line[0] && i++)
			continue ;
		ft_history_loop(shell, lines[i]);
		if (!ft_loop(shell, i + lines, &i, &tok))
			return (ft_free_arr(lines), false);
		i++;
		while (*cmd_line && ft_isspace(*cmd_line))
			cmd_line++;
		ft_parsing_and_execute(shell, cmd_line, tok);
	}
	return (ft_free_arr(lines), true);
}

/**
 * @brief The interactive prompt loop.
 *
 * Continuously reads input from the user via readline, handles signals,
 * calls the brain function, and gracefully exits on EOF (Ctrl+D).
 *
 * @param shell Pointer to the shell structure.
 */
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

/**
 * @brief Entry point of minishell.
 *
 * Initializes the shell, environment, history, signals, and launches the
 * prompt loop. On exit, it performs a final cleanup and returns the shell's
 * exit status.
 *
 * @param argc Argument count (unused).
 * @param argv Argument values (unused).
 * @param envp Environment variables from the system.
 * @return Exit status of the shell.
 */
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
	return (shell.exit_status);
}
