/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:06:53 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/20 20:53:53 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                             HISTORY UTILITIES                              */
/*       Functions for managing memory and printing command history           */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Frees all memory allocated for the history linked list.
 * @param history Address of the pointer to the head of the list.
 */
void	free_history(t_hist **history)
{
	t_hist	*next;

	if (!history || !*history)
		return ;
	while (*history)
	{
		next = (*history)->next;
		ft_free(&(*history)->cmd);
		free(*history);
		*history = next;
	}
	*history = NULL;
}

/**
 * @brief Frees a string and sets its pointer to NULL.
 * @param ptr Address of the pointer to the string to free.
 */
void	ft_free(char **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

/**
 * @brief Recursively prints the history in chronological order.
 * @param history Address of the pointer to the history list.
 * @param i Address of the counter used to number the commands.
 */
void	ft_print_history(t_hist **history, size_t *i)
{
	t_hist	*cur;

	if (!history || !*history)
		return ;
	cur = *history;
	if (cur->next)
		ft_print_history(&cur->next, i);
	if (cur->cmd)
	{
		ft_putstr_fd("    ", STDOUT_FILENO);
		ft_putnbr_fd(*i, STDOUT_FILENO);
		ft_putstr_fd("\t", STDOUT_FILENO);
		ft_putendl_fd(cur->cmd, STDOUT_FILENO);
	}
	(*i)++;
}

void	ft_history_actualise(t_shell *shell)
{
	size_t	histsizemax;

	histsizemax = ft_atoi(get_env_value("HISTSIZE", shell->env));
	if (histsizemax > 0)
	{
		keep_n_hist(&shell->history, histsizemax);
		rl_clear_history();
		load_to_shell_history(&shell->history);
	}
	else
	{
		free_history(&shell->history);
		rl_clear_history();
	}
}
