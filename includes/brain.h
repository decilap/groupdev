/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:17:50 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 11:52:14 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_H
# define BRAIN_H

bool	ft_loop(t_shell *shell, char **line, int *i, t_token **tok);
bool	ft_init_loop_herdoc(char **line, t_token **tok, int *idx, char *delim);
bool	ft_manage_last_line(t_token *scan, char **result);

#endif