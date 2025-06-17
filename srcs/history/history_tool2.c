/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_tool2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:15:10 by ryoussfi          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/17 18:40:51 by ryoussfi         ###   ########.fr       */
=======
/*   Updated: 2025/06/17 13:42:41 by ryoussfi         ###   ########.fr       */
>>>>>>> origin/main
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                          GET NEXT LINE UTILS                               */
/*       Functions to read a line from a file descriptor without \\n          */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Appends a character to a dynamically allocated string.
 * @param str The original string to append to (can be NULL).
 * @param buff The character to append.
 * @return A new string with the appended character, or NULL on malloc( fail.
 */
static char	*ft_stradd(char *str, char buff)
{
	size_t		i;
	char		*ret;
	size_t		len;

	i = 0;
	len = ft_strlen(str);
	ret = (char *)malloc(sizeof(char) * (len + 2));
	if (ret == NULL)
	{
		free(str);
		return (NULL);
	}
	while (str && str[i])
	{
		ret[i] = str[i];
		i++;
	}
	free(str);
	str = NULL;
	ret[i] = buff;
	ret[++i] = '\0';
	return (ret);
}

/**
 * @brief Reads a line from a file descriptor, excluding the newline.
 * @param fd The file descriptor to read from.
 * @param str Pointer to a string pointer where the line is stored (allocated).
 * @return Number of bytes read, or -1 if an error occurs.
 */
ssize_t	gnl(int fd, char **str)
{
	char			buff;
	ssize_t			ret;
	ssize_t			st;

	buff = '\0';
	if (!str)
		return (-1);
	st = read(fd, &buff, 1);
	if (st < 0)
		return (-1);
	ret = st;
	while (st > 0)
	{
		if (buff == '\n')
			return (ret);
		*str = ft_stradd(*str, buff);
		if (!*str)
			return (-1);
		st = read(fd, &buff, 1);
		ret += st;
	}
	return (ret);
}

/**
 * @brief Handles the shell's command history on each new command input.
 *
 * This function adds the current input line to the shell's history, checks if
 * the history exceeds the size defined by the HISTSIZE environment variable,
 * and trims the history if necessary. It also syncs the shell's internal 
 * history with the readline library.
 *
 * @param shell A pointer to the shell structure containing history and env data.
 * @param line The input line to be added to the history.
 */
void	ft_history_loop(t_shell *shell, char *line)
{
	size_t	histsizemax;
	size_t	len_hist;

	histsizemax = ft_atoi(get_env_value("HISTSIZE", shell->env));
	if (histsizemax > 0)
	{
		shell->hist_len++;
		if (!add_to_history(&shell->history, line))
			perror(RED "minishell: Error in add_to_history" RESET);
		add_history(line);
		len_hist = hist_len(shell->history);
		if (len_hist > histsizemax)
		{
			keep_n_hist(&shell->history, histsizemax);
			rl_clear_history();
			load_to_shell_history(&shell->history);
		}
	}
}

/**
 * @brief Adjusts the shell's history size according to the HISTSIZE variable.
 *
 * This function checks if the current history size exceeds the limit defined
 * by the HISTSIZE environment variable. If so, it trims the history and 
 * reloads it into the readline library. If HISTSIZE is zero or invalid, the 
 * entire history is cleared.
 *
 * @param shell A pointer to the shell structure containing history and 
 * environment data.
 */
void	ft_history_change_size(t_shell *shell)
{
	size_t	len_hist;
	size_t	histsizemax;

	histsizemax = ft_atoi(get_env_value("HISTSIZE", shell->env));
	if (histsizemax > 0)
	{
		len_hist = hist_len(shell->history);
		if (len_hist > histsizemax)
		{
			keep_n_hist(&shell->history, histsizemax);
			rl_clear_history();
			load_to_shell_history(&shell->history);
		}
	}
	else
	{
		free_history(&shell->history);
		rl_clear_history();
	}
}

int	ft_history_here_doc(t_shell *shell, char *line)
{
	size_t	histsizemax;
	size_t	len;
	char	*temp;

	if (shell && (!shell->history || !shell->history->cmd))
		return (0);
	histsizemax = ft_atoi(get_env_value("HISTSIZE", shell->env));
	if (histsizemax > 0)
	{
		shell->hist_len++;
		temp = ft_strjoin(shell->history->cmd, line);
		if (!temp)
			return (-1);
		free(shell->history->cmd);
		shell->history->cmd = temp;
		rl_clear_history();
		load_to_shell_history(&shell->history);
		len = hist_len(shell->history);
		if (len > histsizemax)
			ft_history_actualise(shell);
	}
	return (0);
}
