/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_2_loop_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:02 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 14:32:36 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

t_token	*ft_take_delim(t_token *scan, char *delim)
{
	while (scan && scan->value)
	{
		if (ft_strcmp(scan->value, delim) == 0)
			break ;
		scan = scan->next;
	}
	return (scan);
}

static bool	ft_safe_joinmaj(char **s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(*s1, s2);
	if (!tmp)
		return (false);
	free(*s1);
	*s1 = tmp;
	return (true);
}

bool	ft_manage_last_line(t_token *scan, char **result)
{
	char	*tmp;

	if (scan)
	{
		scan = scan->next;
		if (scan)
			if (!ft_safe_joinmaj(result, " && "))
				return (false);
	}
	while (scan)
	{
		if (!ft_safe_joinmaj(result, scan->value))
			return (false);
		if (scan->next && scan->value[0])
			if (!ft_safe_joinmaj(result, " && "))
				return (false);
		scan = scan->next;
	}
	return (true);
}

/**
 * @brief Handles the parsing and expansion of a heredoc token.
 *
 * Processes a heredoc token, performs input capture until the delimiter,
 * and updates the command's input FD accordingly.
 *
 * @param tok      Pointer to the current token (will be advanced).
 * @param shell    Shell context (for status and env).
 * @param curr     The command struct to set the heredoc FD on.
 * @return 1 if heredoc processed, -1 on error, 0 if not a heredoc.
 */
int	handle_heredoc_token(t_token **tok, t_shell *shell, t_cmd *curr,
		t_token *multi_data)
{
	if ((*tok)->type != TOKEN_HEREDOC)
		return (0);
	if (!(*tok)->next)
	{
		ft_putstr_fd(RED "minishell: syntax error near\
			unexpected token `newline'\n" RESET, STDERR_FILENO);
		shell->exit_status = 2;
		return (-1);
	}
	if (handle_here_doc(shell, curr, *tok, multi_data))
		return (-1);
	*tok = (*tok)->next;
	*tok = (*tok)->next;
	return (1);
}
