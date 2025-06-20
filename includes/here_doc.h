/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:20:40 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/20 20:41:56 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

# include <sys/ioctl.h>
# include <unistd.h>

typedef struct s_param
{
	char		*ptrdelim;
	int			quoted;
}	t_param;

typedef struct s_pending
{
	int			first;
	int			write;
}	t_pending;

int		ft_free_go_her(char *line);

void	ft_init_signal_heredoc(struct sigaction *sa_old);
int		ft_end_heredoc(t_cmd *cmd, t_shell *shell);
t_param	ft_init_param(t_shell *shell, t_token *tok, int *fd);

int		handle_here_doc(t_shell *shell, t_cmd *cmd, t_token *tok,
			t_token *multi_data);

void	ft_manage_hist_here_doc(t_shell *shell, char *line);
bool	ft_output_heredoc(t_shell *shell, char **line, int quoted, int fd);
bool	go_heredoc(t_shell *shell, const char *delim, int quoted, int fd);

#endif
