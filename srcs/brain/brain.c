/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:05 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/13 10:19:45 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static void	ft_parsing_and_execute(t_shell *shell, char *result, t_token *tok)
{
	t_cmd	*cmd;

	cmd = parse_input(result, shell, tok);
	if (!cmd)
		return ;
	execute_with_logical(cmd, shell);
	clear_subshell_table();
}

bool	ft_loop_with_herdoc(t_shell *shell, char *line)
{
	t_token	*scan;
	t_token	*tok;
	char	*result;
	char	*delim;

	scan = NULL;
	tok = NULL;
	result = NULL;
	if (!ft_init_loop_heredoc(line, &tok, &delim, &result))
		return (perror(RED "minishell: Error in ft_init_loop_heredoc" RESET),
			false);
	ft_history_loop_newline(shell, tok->value);
	scan = ft_take_delim(tok->next, delim);
	free(delim);
	if (!ft_manage_last_line(scan, &result))
		return (perror(RED "minishell: Error in ft_manage_last_line" RESET),
			free_tokens(tok), free(result), false);
	ft_parsing_and_execute(shell, result, tok);
	return (free_tokens(tok), free(result), true);
}

bool	ft_loop(t_shell *shell, char *line)
{
	char	**lines;
	char	*cmd_line;
	t_cmd	*cmds;
	int		i;

	lines = ft_split(line, '\n');
	if (!lines)
		return (perror(RED "minishell: (ftl) ft_split" RESET),
			false);
	i = 0;
	while (lines[i])
	{
		cmd_line = lines[i];
		if ((!*cmd_line || !cmd_line[0]) && i++)
			continue ;
		ft_history_loop_newline(shell, cmd_line);
		while (*cmd_line && ft_isspace(*cmd_line))
			cmd_line++;
		ft_parsing_and_execute(shell, cmd_line, NULL);
		i++;
	}
	ft_free_arr(lines);
	return (true);
}

static bool	ft_herdoc_chr(char *line, char **eof_delim)
{
	char	**first_line_words;
	int		i;

	first_line_words = ft_split(line, ' ');
	if (!first_line_words)
		return (false);
	i = 0;
	while (first_line_words && first_line_words[i])
	{
		if (ft_strcmp(first_line_words[i], "<<") == 0
			&& first_line_words[i + 1])
		{
			*eof_delim = ft_strdup(first_line_words[i + 1]);
			if (!*eof_delim)
				return (ft_free_arr(first_line_words), false);
			break ;
		}
		i++;
	}
	ft_free_arr(first_line_words);
	return (true);
}

int	has_complete_multiline_heredoc(char *input)
{
	char	**lines;
	char	*eof_delim;
	int		j;

	lines = ft_split(input, '\n');
	if (!lines)
		return (perror(RED "minishell: (hcmh) ft_split" RESET), -1);
	if (!lines[1])
		return (ft_free_arr(lines), 0);
	eof_delim = NULL;
	if (!ft_herdoc_chr(lines[0], &eof_delim))
		return (perror(RED "minishell: ft_herdoc_chr" RESET),
			ft_free_arr(lines), -1);
	if (!eof_delim)
		return (ft_free_arr(lines), 0);
	j = 1;
	while (lines[j])
	{
		if (ft_strcmp(lines[j], eof_delim) == 0)
			return (free(eof_delim), ft_free_arr(lines), 1);
		j++;
	}
	return (free(eof_delim), ft_free_arr(lines), 2);
}
