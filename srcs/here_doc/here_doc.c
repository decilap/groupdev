/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:40:48 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/16 14:57:02 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static char	*line_expansion(char *line, t_shell *shell)
{
	t_token	*tmp;
	char	*str;

	tmp = expand_variables_heredoc(line, shell, 1, Q_HEREDOC);
	if (!tmp)
		return (NULL);
	str = ft_strdup(tmp->value);
	free_tokens(tmp);
	free(line);
	line = NULL;
	return (str);
}

bool	ft_output_heredoc(t_shell *shell, char **line, int quoted, int fd)
{
	if (!quoted)
		*line = line_expansion(*line, shell);
	if (!*line)
	{
		perror(RED "minishell: ft_output_heredoc" RESET);
		shell->exit_status = 1;
		return (false);
	}
	ft_putstr_fd(*line, fd);
	return (true);
}

static int	ft_multi_data(t_shell *shell, t_token *scan, t_param param, int fd)
{
	char	*line;
	int		ret;

	ret = 0;
	line = NULL;
	while (scan && scan->value)
	{
		line = ft_strjoin(scan->value, "\n");
		if (!line)
			return (-1);
		ft_manage_hist_here_doc(shell, line);		
		if (ft_strcmp(line, param.ptrdelim) == 0)
		{
			ret = 1;
			break ;
		}
		if (!ft_output_heredoc(shell, &line, param.quoted, fd))
			return (free(line), -1);
		free(line);
		line = NULL;
		scan = scan->next;
	}
	return (free(line), ret);
}

static int	ft_error_here_doc(t_shell *shell, t_param param, struct sigaction *sa_old, int fd)
{
	safe_close(fd);
	free(param.ptrdelim);
	sigaction(SIGINT, sa_old, NULL);
	shell->exit_status = 1;
	return (-1);
}

int	handle_here_doc(t_shell *shell, t_cmd *cmd, t_token *tok,
		t_token *multi_data)
{
	struct sigaction	sa_old;
	int					ret_multi;
	t_param				param;
	int					fd;

	ret_multi = 0;
	fd = -1;
	param = ft_init_param(shell, tok, &fd);
	if (fd == -1)
		return (-1);
	ft_history_here_doc(shell, "\n");
	if (multi_data)
	{
		ret_multi = ft_multi_data(shell, multi_data->next, param, fd);
		if (ret_multi == -1)
			return (ft_error_here_doc(shell, param, &sa_old, fd));
	}
	ft_init_signal_heredoc(&sa_old);
	if (ret_multi == 0 && !*get_heredoc_interrupt_flag()
			&& !go_heredoc(shell, param.ptrdelim, tok->next->quoted, fd))
		return (ft_error_here_doc(shell, param, &sa_old, fd));
	safe_close(fd);
	sigaction(SIGINT, &sa_old, NULL);
	return (free(param.ptrdelim), ft_end_heredoc(cmd, shell));
}
