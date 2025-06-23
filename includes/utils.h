/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:55 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 13:13:49 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

const char	*handle_separator(const char **str, char **result);
const char	*handle_word(const char **str, char c, char **result);
void print_tokens(t_token *tok);
void	print_env(char **env);


char		**ft_split_new_line(char const *str, char c, t_shell *shell);

void		ft_init_pending(t_pending *pen);
void		ft_error_brain(t_shell *shell);
int			pending_input(void);
int			parse_echo_flags(char **args, int *newline);
void		free_tmp_args(char **args, int count);
void		exit_error(char *msg, t_shell *shell);
void		*safe_malloc(size_t size, t_shell *shell);
void		free_tokens(t_token *tokens);
void		free_cmds(t_cmd *cmd);
void		free_groups(t_group *group);
void        free_one_token(t_token *tok);
void		ft_init_history_env(t_shell *shell);
void		ft_init_shlvl_env(t_shell *shell);
void		ft_init_shell_struct(t_shell *shell, char **envp);
void		print_centered_ascii_art(void);
bool		safe_close(int fd);
bool		safe_write(int fd, char *buf, size_t n);
bool		is_export_builtin(t_cmd *cmd);
bool		is_export_print_mode(t_cmd *cmd);
bool		safe_close(int fd);
char		*ft_strjoin_3(const char *s1, const char *s2, const char *s3);
char		*ft_strjoin_3(const char *s1, const char *s2, const char *s3);

#endif
