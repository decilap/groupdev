/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:19:50 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:43:02 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# define TMP_FILE_HIST "/tmp/.minishell_history"
# define FILE_HIST ".minishell_history"

/*			HISTORY_INIT.C			*/
size_t	hist_len(t_hist *history);
void	keep_n_hist(t_hist **history, size_t sizemax);
bool	add_to_history(t_hist **history, char *cmd);
void	load_to_shell_history(t_hist **history);
bool	initialize_history(t_shell *shell, t_hist **history);

/*			HISTORY_SAVE.C			*/
bool	save_to_file(t_shell *shell, t_hist **history);
void	ft_history_loop_newline(t_shell *shell, char *line);

/*			HISTORY_TOOL.C			*/
bool	invert_hist(t_hist **history);
bool	write_on_file(int fd, t_hist *history, size_t *filesize);
bool	cut_invert_write_on_file(int fd, t_hist **history, size_t sizemax);
bool	history_from_file(int fd, t_hist **history, size_t sizemax);
void	ft_history_actualise(t_shell *shell);

/*			HISTORY_TOOL2.C			*/
ssize_t	gnl(int fd, char **str);
void	ft_history_loop(t_shell *shell, char *line);
void	ft_history_change_size(t_shell *shell);
int		ft_history_here_doc(t_shell *shell, char *line);

/*			HISTORY_UTILS.C			*/
void	free_history(t_hist **history);
void	ft_free(char **ptr);
void	ft_print_history(t_hist **history, size_t *i);
void	print_hist(t_hist *history);

#endif