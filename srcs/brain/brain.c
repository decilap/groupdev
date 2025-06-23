/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:05 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 17:50:36 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static t_token	*ft_loop_with_herdoc(char **line, int *i, char *delim,
		t_shell *shell)
{
	t_token	*tok;

	tok = NULL;
	if (!ft_init_loop_herdoc(line, &tok, i, delim, shell))
	{
		return (perror(RED "minishell: Error in ft_init_loop_herdoc" RESET),
			NULL);
	}
	return (tok);
}

static bool	ft_herdoc_chr(char *line, char **eof_delim, t_shell *shell)
{
	t_token	*cmd;

	cmd = lexer(line, shell);
	if (!cmd)
		return (false);
	while (cmd && cmd->value)
	{
		if (ft_strcmp(cmd->value, "<<") == 0)
		{
			if (!cmd->next || !cmd->next->value)
				return (free_tokens(cmd), false);
			*eof_delim = ft_strdup(cmd->next->value);
			if (!*eof_delim)
				return (free_tokens(cmd), false);
			break ;
		}
		cmd = cmd->next;
	}
	free_tokens(cmd);
	return (true);
}

static int	has_complete_multiline_heredoc(char **lines, char **delim,
		t_shell *shell)
{
	int		j;
	char	*eof_delim;

	eof_delim = NULL;
	if (!lines[1])
		return (0);
	if (!ft_herdoc_chr(lines[0], &eof_delim, shell))
		return (perror(RED "minishell: ft_herdoc_chr" RESET), -1);
	if (!eof_delim)
		return (0);
	*delim = ft_strdup(eof_delim);
	if (!*delim)
		return (free(eof_delim), perror(RED "minishell: ft_strdup" RESET), -1);
	j = 1;
	while (lines[j])
	{
		if (ft_strcmp(lines[j], eof_delim) == 0)
			return (free(eof_delim), 1);
		j++;
	}
	return (free(eof_delim), 2);
}

bool	ft_loop(t_shell *shell, char **line, int *i, t_token **tok)
{
	int		ret_multi;
	char	*delim;

	delim = NULL;
	ret_multi = has_complete_multiline_heredoc(line, &delim, shell);
	if (ret_multi < 0)
	{
		shell->exit_status = 1;
		perror(RED "minishell: Error in has_complete_multiline_heredoc" RESET);
		return (false);
	}
	if (ret_multi > 0)
	{
		*tok = ft_loop_with_herdoc(line, i, delim, shell);
		if (*tok)
			return (true);
		shell->exit_status = 1;
		perror(RED "minishell: Error in ft_loop_with_herdoc" RESET);
		return (false);
	}
	return (true);
}
