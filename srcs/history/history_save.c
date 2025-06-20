/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_save.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:20:35 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/20 20:40:39 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                           HISTORY MANAGEMENT                               */
/*       Functions for Saving and Managing Command History in Minishell      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Writes the history data to the specified file.
 * @param f_name File name to write history data to.
 * @param l_hist Pointer to the last history list.
 * @param hist Pointer to the current history list.
 * @param s_max Maximum size of the history to be written.
 * @return true if successful, false otherwise.
 */
static bool	write_hist(char *f_name, t_hist **l_hist, t_hist **hist,
		size_t s_max)
{
	int			fd;

	fd = open(f_name, O_WRONLY | O_TRUNC);
	if (fd < 0)
	{
		perror(RED "minishell: Failed To Open And Trunc History File And Save Da\
ta" RESET);
		return (false);
	}
	if (!write_on_file(fd, *l_hist, &s_max))
	{
		safe_close(fd);
		return (false);
	}
	if (!cut_invert_write_on_file(fd, hist, s_max))
	{
		safe_close(fd);
		return (false);
	}
	safe_close(fd);
	return (true);
}

/**
 * @brief Initializes the history from a file, limited by the specified size.
 * @param f_name File name to read history from.
 * @param hist Pointer to the history list.
 * @param s_max Maximum size of the history to be read.
 * @return true if successful, false otherwise.
 */
static bool	init_hist_from_file(char *f_name, t_hist **hist, size_t s_max)
{
	int		fd;

	if (s_max == 0)
		return (true);
	fd = open(f_name, O_RDONLY);
	if (fd < 0)
	{
		perror(RED "minishell: Unable To Open History File And Retrieve\
			Data" RESET);
		return (false);
	}
	if (!history_from_file(fd, hist, s_max))
	{
		safe_close(fd);
		return (false);
	}
	safe_close(fd);
	invert_hist(hist);
	return (true);
}

/**
 * @brief Manages the history operations, including file truncation and merging.
 * @param env Environment variables array.
 * @param l_hist Pointer to the last history list.
 * @param hist Pointer to the current history list.
 * @param len_hist Length of the current history list.
 * @return true if successful, false otherwise.
 */
static bool	manage_hist(char **env, t_hist **l_hist, t_hist **hist,
		size_t len_hist)
{
	size_t		sizemax;
	size_t		trunc_status;
	char		*filename;

	sizemax = ft_atoi(get_env_value("HISTFILESIZE", env));
	trunc_status = ft_atoi(get_env_value("HISTTRUNC", env));
	filename = get_env_value("HISTFILE", env);
	if (!filename)
		return (true);
	if (!trunc_status && sizemax > len_hist
		&& !init_hist_from_file(filename, l_hist, sizemax - len_hist))
		return (false);
	if (!write_hist(filename, l_hist, hist, sizemax))
		return (false);
	return (true);
}

/**
 * @brief Saves the history to a file.
 * @param shell Pointer to the shell structure.
 * @param history Pointer to the history list.
 * @return true if successful, false otherwise.
 */
bool	save_to_file(t_shell *shell, t_hist **history)
{
	t_hist		*last_hist;
	size_t		len_hist;
	size_t		histsizemax;

	last_hist = NULL;
	len_hist = hist_len(*history);
	histsizemax = ft_atoi(get_env_value("HISTSIZE", shell->env));
	if (shell->hist_len == 0)
		free_history(history);
	else
		keep_n_hist(history, shell->hist_len);
	if (shell->hist_len >= histsizemax)
		set_env_value("HISTTRUNC", "1", shell);
	if (!manage_hist(shell->env, &last_hist, history, len_hist))
	{
		free_history(&last_hist);
		shell->exit_status = 1;
		return (false);
	}
	free_history(&last_hist);
	return (true);
}
