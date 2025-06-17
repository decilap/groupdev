/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:01:54 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/17 13:42:59 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Prints an error message and exits the program.
 * 
 * @param msg The error message to display.
 */
void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * @brief Allocates memory safely. Exits the program if allocation fails.
 * 
 * @param size The size of memory to allocate.
 * @return A pointer to the allocated memory.
 */
void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		exit_error(RED "minishell: 00 unset" RESET);
	return (ptr);
}

void	ft_error_brain(t_shell *shell)
{
	perror(RED "minishell: Error in ft_brain_of." RESET);
	shell->exit_status = 1;
}

void	ft_init_pending(t_pending *pen)
{
	pen->first = 1;
	pen->write = 1;
}

// Renvoie le nombre d'octets prêts à être lus dans STDIN
int	pending_input(void)
{
	int	count;

	count = 0;
	ioctl(STDIN_FILENO, FIONREAD, &count);
	return (count);
}
