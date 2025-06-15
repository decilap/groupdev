/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:07:49 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 16:26:06 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static void	ft_output_ctrl_d(t_shell *shell, size_t i)
{
	char	*msg;
	char	*index;

	if (shell && shell->history)
		index = ft_itoa(shell->hist_len + i);
	else
		index = ft_itoa(i);
	msg = ft_strjoin_3("\nbash: warning: here-document at line ", index, \
				" delimited by end-of-file (wanted `EOF')\n");
	ft_putstr_fd(msg, STDOUT_FILENO);
	if (!index)
	{
		perror(RED "minishell: (idx) ft_output_ctrl_d" RESET);
		shell->exit_status = 1;
		return ;
	}
	free(index);
	if (!msg)
	{
		perror(RED "minishell: (msg) ft_output_ctrl_d" RESET);
		shell->exit_status = 1;
		return ;
	}
	free(msg);
}

void	ft_manage_hist_here_doc(t_shell *shell, char *line)
{
	char	*cpy_line;

	cpy_line = ft_strdup(line);
	if (!cpy_line)
	{
		perror(RED "minishell: (cpy) ft_manage_hist_here_doc" RESET);
		shell->exit_status = 1;
		return ;
	}
	if (ft_history_here_doc(shell, cpy_line))
	{
		free(cpy_line);
		perror(RED "minishell: ft_manage_hist_here_doc" RESET);
		shell->exit_status = 1;
		return ;
	}
	free(cpy_line);
}

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


// Renvoie le nombre d'octets prêts à être lus dans STDIN
/* int	pending_input(void)
{
	int	count;

	count = 0;
	ioctl(STDIN_FILENO, FIONREAD, &count);
	return (count);
} */

bool	go_heredoc(t_shell *shell, const char *delim, int quoted, int fd)
{
	char	*line;
	size_t	i;
	int		stand;
	// int		first;
	// int		write;

	i = 0;
	// first = 1;
	// write = 1;
	while (!*get_heredoc_interrupt_flag())
	{
		// stand = pending_input();
		// if (stand > 0)
		// 	write = 0;
		// if (first || (isatty(STDIN_FILENO) && stand == 0))
		// {
		// 	first = 0;
		// 	if (write == 0)
		// 		write = 1;
		// 	else
				ft_putstr_fd("> ", STDOUT_FILENO);
		// }
		line = get_next_line(STDIN_FILENO);
		// printf("<==========%s===========>\n", line);
		if (!line)
		{
			if (!*get_heredoc_interrupt_flag())
				ft_output_ctrl_d(shell, i);
			return (true);
		}
		ft_manage_hist_here_doc(shell, line);
		if (ft_strcmp(line, delim) == 0)
			break ;
		if (!ft_output_heredoc(shell, &line, quoted, fd))
			return (false);
		free(line);
		i++;
	}
	free(line);
	return (true);
}
