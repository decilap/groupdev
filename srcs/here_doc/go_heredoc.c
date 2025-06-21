/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:07:49 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/21 16:06:46 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

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

static void	ft_output_ctrl_d(t_shell *shell, size_t i)
{
	char	*msg;
	char	*index;

	if (shell && shell->history)
		index = ft_itoa(shell->hist_len + i);
	else
		index = ft_itoa(i);
	if (!index || !shell)
	{
		perror(RED "minishell: (idx) ft_output_ctrl_d" RESET);
		if (shell)
			shell->exit_status = 1;
		return ;
	}
	msg = ft_strjoin_3("\nbash: warning: here-document at line ", index,
			" delimited by end-of-file (wanted `EOF')\n");
	free(index);
	if (!msg)
	{
		perror(RED "minishell: (msg) ft_output_ctrl_d" RESET);
		shell->exit_status = 1;
		return ;
	}
	ft_putstr_fd(msg, STDOUT_FILENO);
	free(msg);
}

char	*line_expansion(char *line, t_shell *shell)
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

static void	ft_put_prompt(int *first, int *write, int *copy_past)
{
	int		stand;
	int		is;
	int		no;

	stand = 0;
	is = 0;
	no = 0;
	stand = pending_input();
	if (stand > 0)
		*write = 0;
	is = isatty(STDIN_FILENO);
	if (stand)
		*copy_past = 1;
	if (*copy_past && !*first && *write && is && !stand)
		no = 1;
	if (!no && (*first || (is && stand == 0)))
	{
		*first = 0;
		if (*write == 0)
			*write = 1;
		else
			ft_putstr_fd("> ", STDOUT_FILENO);
	}
	*copy_past = 0;
}

bool	go_heredoc(t_shell *shell, const char *delim, int quoted, int fd)
{
	char		*line;
	size_t		i;
	t_pending	pen;

	i = 0;
	ft_init_pending(&pen);
	while (!*get_heredoc_interrupt_flag() && ++i)
	{
		ft_put_prompt(&pen.first, &pen.write, &pen.cpy_pst);
		line = get_next_line_mod(STDIN_FILENO);
		if (!line)
		{
			if (!*get_heredoc_interrupt_flag())
				ft_output_ctrl_d(shell, i);
			return (true);
		}
		ft_manage_hist_here_doc(shell, line);
		if (ft_strcmp(line, delim) == 0 && ft_free_go_her(line))
			break ;
		if (!ft_output_heredoc(shell, &line, quoted, fd))
			return (false);
		free(line);
	}
	return (true);
}
