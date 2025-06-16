/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 21:10:44 by san               #+#    #+#             */
/*   Updated: 2025/06/15 12:04:13 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

void	ft_init_signal_heredoc(struct sigaction *sa_old)
{
	struct sigaction	sa_new;

	sa_new.sa_handler = sigint_here_doc_handler;
	sa_new.sa_flags = 0;
	sigemptyset(&sa_new.sa_mask);
	sigaction(SIGINT, &sa_new, sa_old);
	*get_heredoc_interrupt_flag() = 0;
}

static bool	ft_init_delimiter(t_shell *shell, const char *delimiter,
	char **clean_delim, int quoted)
{
	char	*tmp;

	*clean_delim = ft_strdup(delimiter);
	if (!*clean_delim)
	{
		shell->exit_status = 1;
		return (-1);
	}
	if (quoted)
		remove_quotes(clean_delim);
	if (!*clean_delim)
	{
		shell->exit_status = 1;
		return (-1);
	}
	tmp = ft_strjoin(*clean_delim, "\n");
	if (!tmp)
	{
		free(*clean_delim);
		shell->exit_status = 1;
		return (-1);
	}
	free(*clean_delim);
	*clean_delim = tmp;
	return (0);
}

static int	ft_init_fd_heredoc(t_shell *shell, int *fd, char **ptrdelim, t_token *tok)
{
	int	fd_tmp;

	fd_tmp = open(TMP_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd_tmp < 0)
	{
		perror("heredoc: open temp");
		shell->exit_status = 1;
		return (-1);
	}
	*fd = fd_tmp;
	if (ft_init_delimiter(shell, tok->next->value, ptrdelim, tok->next->quoted))
	{
		perror("heredoc: ft_init_delimiter");
		safe_close(*fd);
		shell->exit_status = 1;
		return (-1);
	}
	return (*fd);
}

int	ft_end_heredoc(t_cmd *cmd, t_shell *shell)
{
	if (*get_heredoc_interrupt_flag())
	{
		cmd->fd_in = open(TMP_FILE, O_TRUNC | O_RDONLY);
		unlink(TMP_FILE);
		if (cmd->fd_in < 0)
		{
			perror("heredoc: reopen");
			return (-1);
		}
		shell->exit_status = 130;
	}
	else
	{
		cmd->fd_in = open(TMP_FILE, O_RDONLY);
		unlink(TMP_FILE);
		if (cmd->fd_in < 0)
		{
			perror("heredoc: reopen");
			return (-1);
		}
		shell->exit_status = 0;
	}
	return (0);
}

t_param	ft_init_param(t_shell *shell, t_token *tok, int *fd)
{
	t_param	param;
	int		fd_tmp;

	param.ptrdelim = NULL;
	if (ft_init_fd_heredoc(shell, &fd_tmp, &(param.ptrdelim), tok) < 0)
		return (param);
	*fd = fd_tmp;
	param.quoted = tok->next->quoted;
	return (param);
}
