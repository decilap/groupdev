/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:48 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 13:48:20 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		is_builtin(t_cmd *cmd);
int		builtin_cd(char **args, t_shell *shell);
int		builtin_echo(char **args);
int		builtin_pwd(char **args);
int		builtin_export(char **args, t_shell *shell);
int		builtin_unset(char **args, t_shell *shell);
int		builtin_env(char **args, t_shell *shell);
int		builtin_exit(char **args, t_shell *shell);
int		execute_builtin(t_cmd *cmd, t_shell *shell);
int		is_valid_identifier(const char *str);
int		run_builtin(t_cmd *cmd, t_shell *shell);
char	*resolve_cd_path(char **args, t_shell *shell);
char	**prepare_sorted_env(char **env);
void	update_pwd_vars(char *oldpwd, t_shell *shell);

#endif
