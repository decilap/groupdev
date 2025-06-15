/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:40:33 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define TMP_FILE "/tmp/.minishell_heredoc_tmp"

void			init_signals(void);
void			sigint_here_doc_handler(int sig);
sig_atomic_t	*get_heredoc_interrupt_flag(void);

#endif
