/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:17:50 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:45:39 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_H
# define BRAIN_H

bool	ft_loop_with_herdoc(t_shell *shell, char *line);
bool	ft_loop(t_shell *shell, char *line);
int		has_complete_multiline_heredoc(char *input);

bool	ft_init_loop_heredoc(char *line, t_token **tok, char **delim,
			char **result);

t_token	*ft_take_delim(t_token *scan, char *delim);
bool	ft_manage_last_line(t_token *scan, char **result);

#endif