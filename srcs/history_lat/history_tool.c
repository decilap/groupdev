/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_tool.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:34:44 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 18:12:08 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                             HISTORY LOADING                                */
/*    Functions for Reading, Writing, and Managing History Data from File     */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Invert the order of the history linked list.
 * @param history Pointer to the pointer of the history list.
 * @return true if successful, false if memory allocation fails.
 */
bool	invert_hist(t_hist **history)
{
	t_hist	*current;
	t_hist	*invert;

	invert = NULL;
	current = *history;
	while (current && current->cmd)
	{
		if (!add_to_history(&invert, current->cmd))
		{
			free_history(&invert);
			return (false);
		}
		current = current->next;
	}
	current = *history;
	*history = invert;
	free_history(&current);
	return (true);
}

/**
 * @brief Write history commands into a file descriptor.
 * @param fd File descriptor to write into.
 * @param history Pointer to the history list to write.
 * @param filesize Pointer to max number of lines to write.
 * @return true if all writes succeed, false on error.
 */
bool	write_on_file(int fd, t_hist *history, size_t *filesize)
{
	size_t		i;
	size_t		len;
	t_hist		*current;

	current = history;
	i = 0;
	while (current && current->cmd && i < *filesize)
	{
		len = ft_strlen(current->cmd);
		if (write(fd, current->cmd, len) < 0)
		{
			perror(RED "minishell: Failed To Write To File cmd" RESET);
			return (false);
		}
		if (write(fd, "\n", 1) <= 0)
		{
			perror(RED "minishell: Failed To Write To File \\n" RESET);
			return (false);
		}
		current = current->next;
		i++;
	}
	*filesize = *filesize - i;
	return (true);
}

/**
 * @brief Trim, invert, and write history to a file.
 * @param fd File descriptor to write into.
 * @param history Pointer to the pointer of the history list.
 * @param sizemax Maximum number of history entries to keep/write.
 * @return true if successful, false on failure.
 */
bool	cut_invert_write_on_file(int fd, t_hist **history, size_t sizemax)
{
	if (!history || !*history || sizemax == 0)
		return (true);
	keep_n_hist(history, sizemax);
	invert_hist(history);
	if (!write_on_file(fd, *history, &sizemax))
		return (false);
	return (true);
}

/**
 * @brief Load history commands from a file descriptor.
 * @param fd File descriptor to read from.
 * @param history Pointer to the pointer of the history list.
 * @return true if successful, false on error.
 */
static bool	load_from_file(int fd, t_hist **history)
{
	char		*str;
	ssize_t		ret_gnl;

	str = NULL;
	while (1)
	{
		ret_gnl = gnl(fd, &str);
		if (ret_gnl <= 0)
			break ;
		if (!add_to_history(history, str))
		{
			ft_free(&str);
			safe_close(fd);
			return (false);
		}
		ft_free(&str);
	}
	ft_free(&str);
	if (ret_gnl < 0)
	{
		perror(RED "minishell: Error in gnl: unset" RESET);
		return (false);
	}
	return (true);
}

/**
 * @brief Load limited history commands from a file.
 * @param fd File descriptor to read from.
 * @param history Pointer to the pointer of the history list.
 * @param sizemax Maximum number of history entries to load.
 * @return true if successful, false on failure.
 */
bool	history_from_file(int fd, t_hist **history, size_t sizemax)
{
	t_hist	*current_hist;

	if (sizemax == 0)
		return (true);
	current_hist = NULL;
	if (!load_from_file(fd, &current_hist))
	{
		free_history(&current_hist);
		safe_close(fd);
		return (false);
	}
	keep_n_hist(&current_hist, sizemax);
	*history = current_hist;
	return (true);
}
