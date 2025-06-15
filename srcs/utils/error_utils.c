/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:01:54 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 16:27:18 by decilapdeni      ###   ########.fr       */
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
/* 
void	ry_an_exit(t_shell *shell, int exit_code, char *msg)
{
	if (!shell)
		exit(1);
	if (!save_to_file(shell, &shell->history))
		perror(RED "minishell: Error in save_to_file for history" RESET);
	if (shell->history)
		free_history(&shell->history);
	rl_clear_history();
	if (shell->cmds)
		free_cmds(shell->cmds);
	if (shell->env)
		ft_free_arr(shell->env);
	if (exit_code)
		perror(msg);
	// if (exit_code == 0)
	// 	exit(shell->exit_status);
	exit(exit_code);
} */
