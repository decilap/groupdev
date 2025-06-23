/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_loop_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:00 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 17:50:44 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

t_token	*tokenize_lines_from_str(char **str, t_shell *shell)
{
	t_token_data	data;
	t_token			*tokens;
	t_token			*check;
	char			*cmd;

	tokens = NULL;
	while (str && *str)
	{
		cmd = ft_strdup(*str);
		if (!cmd)
		{
			free_tokens(tokens);
			return (NULL);
		}
		data = (t_token_data){cmd, TOKEN_WORD, 0, Q_NONE};
		check = add_token(&tokens, data, shell);
		if (!check)
		{
			free_tokens(tokens);
			return (free(cmd), NULL);
		}
		str++;
	}
	return (tokens);
}

static int	ft_take_delim(char **scan, char *delim)
{
	int	i;

	i = 0;
	while (scan && *scan)
	{
		if (ft_strcmp(*scan, delim) == 0)
			return (i);
		scan++;
		i++;
	}
	return (i - 1);
}

bool	ft_init_loop_herdoc(char **line, t_token **tok, int *idx, char *delim,
		t_shell *shell)
{
	int		i;
	t_token	*token;

	*tok = tokenize_lines_from_str(line, shell);
	if (!*tok)
	{
		perror(RED "minishell: Error in tokenize_lines_from_str" RESET);
		return (false);
	}
	token = (*tok);
	while (token && !ft_strcmp("<<", delim))
		token = token->next;
	i = ft_take_delim(1 + line, delim) + 1;
	*idx += i;
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
