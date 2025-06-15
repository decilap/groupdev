/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_tool.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:45:48 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/11 20:14:53 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static void	ft_history_multi_data(t_shell *shell, t_token *scan)
{
	char	*line;

	if (scan && scan->value)
		scan = scan->next;
	while (scan && scan->value)
	{
		line = scan->value;
		if (line[0])
			ft_history_loop(shell, line);
		scan = scan->next;
	}
}

int	ft_multi_data(t_shell *shell, t_token *scan, t_param param)
{
	char	*line;
	int		ret;
	char	*lim;

	ret = 0;
	line = NULL;
	while (scan && scan->value)
	{
		line = ft_strjoin(scan->value, "\n");
		if (!line)
			return (-1);
		ft_manage_hist_here_doc(shell, line);
		if (ft_strcmp(line, param.delim) == 0)
		{
			ret = 1;
			break ;
		}
		if (!ft_output_heredoc(shell, &line, param.quoted, param.fd))
			return (free(line), -1);
		free(line);
		line = NULL;
		scan = scan->next;
	}
	ft_history_multi_data(shell, scan);
	return (free(line), ret);
}

int	ft_error_here_doc(t_shell *shell, char *delim,
		struct sigaction *sa_old, int fd)
{
	free(delim);
	safe_close(fd);
	sigaction(SIGINT, sa_old, NULL);
	shell->exit_status = 1;
	return (-1);
}
