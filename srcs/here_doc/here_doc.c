/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:40:48 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 16:04:01 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static void	ft_init_signal_heredoc(struct sigaction *sa_old)
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
	if (!clean_delim)
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

static int	ft_init_fd_heredoc(t_shell *shell, int *fd,
	char *clean_delim, struct sigaction *sa_old)
{
	int	fd_tmp;

	fd_tmp = open(TMP_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd_tmp < 0)
	{
		perror("heredoc: open temp");
		free(clean_delim);
		sigaction(SIGINT, sa_old, NULL);
		shell->exit_status = 1;
		return (-1);
	}
	*fd = fd_tmp;
	return (fd_tmp);
}

static int	ft_end_heredoc(t_cmd *cmd, t_shell *shell)
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

static t_param	ft_init_param(int *ret_multi, char *delim, int quoted, int fd_tmp)
{
	t_param	param;

	*ret_multi = 0;
	param.delim = delim;
	param.quoted = quoted;
	param.fd = fd_tmp;
	return (param);
}

int	handle_here_doc(t_shell *shell, t_cmd *cmd, t_token *tok,
		t_token *multi_data)
{
	struct sigaction	sa_old;
	char				*delim;
	int					fd_tmp;
	int					ret_multi;
	t_param				param;

	ft_init_signal_heredoc(&sa_old);
	if (ft_init_delimiter(shell, tok->next->value, &delim, tok->next->quoted))
		return (-1);
	if (ft_init_fd_heredoc(shell, &fd_tmp, delim, &sa_old) < 0)
		return (-1);
	param = ft_init_param(&ret_multi, delim, tok->next->quoted, fd_tmp);
	if (multi_data)
	{
		ret_multi = ft_multi_data(shell, multi_data->next, param);
		if (ret_multi == -1)
			return (ft_error_here_doc(shell, delim, &sa_old, fd_tmp));
	}
	if (!*get_heredoc_interrupt_flag() && ret_multi == 0
		&& !go_heredoc(shell, delim, tok->next->quoted, fd_tmp))
		return (ft_error_here_doc(shell, delim, &sa_old, fd_tmp));
	free(delim);
	safe_close(fd_tmp);
	sigaction(SIGINT, &sa_old, NULL);
	return (ft_end_heredoc(cmd, shell));
}

