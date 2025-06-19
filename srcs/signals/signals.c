/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:02:20 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:57:48 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

volatile sig_atomic_t	g_signal = 0;

/**
 * @brief Signal handler for SIGINT (Ctrl + C).
 * 
 * This function updates the global signal variable, resets the readline
 * line buffer, and redisplays the prompt.
 * 
 * @param signo Signal number (ignored in this implementation).
 */
static void	sigint_handler(int signo)
{
	(void)signo;
	g_signal = signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * @brief Initializes signal handling for the shell.
 * 
 * Sets custom handlers for SIGINT and ignores SIGQUIT.
 */
void	init_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

sig_atomic_t	*get_heredoc_interrupt_flag(void)
{
	static sig_atomic_t	interrupted;

	return (&interrupted);
}

void	sigint_here_doc_handler(int sig)
{
	(void)sig;
	*get_heredoc_interrupt_flag() = 1;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
}
