/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:20:40 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 16:27:59 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

typedef struct s_param
{
	char		*delim;
	int			quoted;
	int			fd;
}	t_param;

int		handle_here_doc(t_shell *shell, t_cmd *cmd, t_token *tok,
			t_token *multi_data);
void	ft_manage_hist_here_doc(t_shell *shell, char *line);
bool	ft_output_heredoc(t_shell *shell, char **line, int quoted, int fd);
bool	go_heredoc(t_shell *shell, const char *delim, int quoted, int fd);

int		ft_multi_data(t_shell *shell, t_token *scan, t_param param);
int		ft_error_here_doc(t_shell *shell, char *delim,
			struct sigaction *sa_old, int fd);

#endif
