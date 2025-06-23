/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:12:23 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:29:42 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

t_var	*find_local_var(t_shell *shell, const char *name);
void	set_env_value(const char *name, const char *value, t_shell *shell);
void	set_env_value_local(const char *name, t_shell *shell);
char	*get_shell_var_value(const char *name, t_shell *shell);
void	set_local_var(t_shell *shell, const char *name, const char *value);
void	add_to_env_from_local(t_shell *shell, const char *name);
char	*get_cmd_path(char *cmd, char **env, int *err_code);
char	*get_env_value(const char *name, char **env);
char	**copy_env(t_shell *shell, char **envp);
char	*search_in_path(char *cmd, char **paths);
void	add_to_env(t_shell *shell, char *args);
void	remove_env_var(const char *name, t_shell *shell);

#endif
