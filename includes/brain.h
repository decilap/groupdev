/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:17:50 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/21 18:03:25 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_H
# define BRAIN_H

bool	ft_loop(t_shell *shell, char **line, int *i, t_token **tok);
bool	ft_init_loop_herdoc(char **line, t_token **tok, int *idx, char *delim, t_shell *shell);
bool	ft_manage_last_line(t_token *scan, char **result);

#endif