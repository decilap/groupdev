/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:33:52 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/17 13:25:22 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                           HISTORY INITIALIZATION                           */
/*       Functions for Initializing and Managing Command History Data        */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Computes the length of the history list.
 * @param history Pointer to the head of the history list.
 * @return The number of nodes in the history list.
 */
size_t	hist_len(t_hist *history)
{
	size_t		i;
	t_hist		*cur;

	cur = history;
	i = 0;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

/**
 * @brief Keeps only the last 'sizemax' commands in the history.
 *        Frees older commands beyond the specified size.
 * @param history Pointer to the history list.
 * @param sizemax Maximum number of commands to retain.
 */
void	keep_n_hist(t_hist **history, size_t sizemax)
{
	t_hist	*cur;
	t_hist	*to_free;

	if (!history || !*history)
		return ;
	if (sizemax == 0)
		return ;
	cur = *history;
	while (cur && --sizemax > 0)
		cur = cur->next;
	if (cur && cur->next)
	{
		to_free = cur->next;
		cur->next = NULL;
		free_history(&to_free);
	}
}

/**
 * @brief Adds a new command to the history list.
 * @param history Pointer to the history list.
 * @param cmd Command string to be added.
 * @return true if the command was successfully added, false otherwise.
 */
bool	add_to_history(t_hist **history, char *cmd)
{
	t_hist	*new_node;

	new_node = mallocsizeof(t_hist));
	if (!new_node)
	{
		perror(RED "minishell: 6 unset" RESET);
		return (false);
	}
	new_node->cmd = ft_strdup(cmd);
	if (!new_node->cmd)
	{
		free(new_node);
		perror(RED "minishell: 8 unset" RESET);
		return (false);
	}
	new_node->next = *history;
	*history = new_node;
	return (true);
}

/**
 * @brief Loads the custom history into the shell's native history system.
 *        The oldest command is written first.
 * @param history Pointer to the history list.
 */
void	load_to_shell_history(t_hist **history)
{
	t_hist	*cur;

	if (!history || !*history)
		return ;
	cur = *history;
	if (cur->next)
		load_to_shell_history(&cur->next);
	if (cur->cmd)
		add_history(cur->cmd);
}

/**
 * @brief Initializes the history system using the `.minishell_history` file
 *        and the maximum size defined by the HISTSIZE environment variable.
 * @param shell Pointer to the shell structure containing environment data.
 * @param history Pointer to the history list.
 * @return true if initialization is successful, false otherwise.
 */
bool	initialize_history(t_shell *shell, t_hist **history)
{
	int		fd;
	char	*filename;
	size_t	sizemax;

	filename = get_env_value("HISTFILE", shell->env);
	if (!filename || !*filename)
		return (true);
	sizemax = ft_atoi(get_env_value("HISTSIZE", shell->env));
	fd = open(filename, O_RDONLY | O_CREAT, 0666);
	if (fd < 0)
	{
		perror(RED ERR_OPEN RESET);
		shell->exit_status = 1;
		return (false);
	}
	if (!history_from_file(fd, history, sizemax))
	{
		safe_close(fd);
		shell->exit_status = 1;
		return (false);
	}
	load_to_shell_history(history);
	shell->histfile_len = hist_len(*history);
	safe_close(fd);
	return (true);
}
