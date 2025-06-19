/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:50:49 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 22:22:01 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Checks for forbidden use of '~' as a standalone command.
 *
 * If '~' is used alone, prints an error, tries to show $HOME,
 * sets shell->exit_status, and frees memory.
 *
 * @param line   Input command line to check.
 * @param shell  Pointer to the shell context for status update.
 * @return 1 if forbidden case detected, 0 otherwise.
 */
int	is_forbidden_tilde(const char *line, t_shell *shell)
{
	char	*home;

	if (ft_strcmp(line, "~") == 0)
	{
		home = get_env_value("HOME", shell->env);
		ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
		if (home)
			ft_putstr_fd(home, STDERR_FILENO);
		ft_putstr_fd(" : est un dossier\n" RESET, STDERR_FILENO);
		shell->exit_status = 126;
		return (1);
	}
	return (0);
}

/**
 * @brief Checks for forbidden use of '.' and '..' as standalone commands.
 *
 * Prints a specific error message and sets shell->exit_status if forbidden.
 *
 * @param line   Input command line to check.
 * @param shell  Pointer to the shell context for status update.
 * @return 1 if forbidden case detected, 0 otherwise.
 */
int	is_forbidden_dot(const char *line, t_shell *shell)
{
	if (ft_strcmp(line, ".") == 0)
	{
		ft_putstr_fd(RED "minishell: .: nom de fichier nécessaire\
			en argument\n", STDERR_FILENO);
		ft_putstr_fd(" : utilisation : . nom_fichier\
			[arguments]\n" RESET, STDERR_FILENO);
		shell->exit_status = 2;
		return (1);
	}
	if (ft_strcmp(line, "..") == 0)
	{
		ft_putstr_fd(RED ".. : commande introuvable\n" RESET, STDERR_FILENO);
		shell->exit_status = 127;
		return (1);
	}
	return (0);
}

/**
 * @brief Checks for repeated special operator syntax (e.g. &&, ||, ;;).
 *
 * If the line starts with two or more of the same special character,
 * prints a syntax error message and returns 1.
 *
 * @param line   Input line to check.
 * @param type   The special character to check (e.g. '&', '|', ';').
 * @return 1 if syntax error detected, 0 otherwise.
 */
int	check_special_syntax(const char *line, char type)
{
	int		i;
	int		count;
	char	*s;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == type)
	{
		count = 1;
		while (line[i + count] == type && count < 2)
			count++;
		s = ft_substr(line + i, 0, count);
		ft_putstr_fd(RED "minishell: syntax error near unexpected token\
			`", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putstr_fd("'\n" RESET, STDERR_FILENO);
		free(s);
		return (1);
	}
	return (0);
}

/**
 * @brief Frees partially allocated argument array on strdup failure.
 *
 * This function is used to avoid memory leaks when some arguments
 * have already been allocated but an allocation fails mid-process.
 *
 * @param args The argument array.
 * @param count Number of already allocated arguments to free.
 */
void	free_partial_args(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(args[i]);
		i++;
	}
}

/**
 * @brief Extracts the first word from the input string.
 *
 * This function scans the input until it reaches the first whitespace,
 * and returns a new allocated substring containing only the first word.
 *
 * @param line The input string to process.
 * @return A newly allocated string containing the first word.
 */
char	*extract_first_word(const char *line)
{
	int	i;

	i = 0;
	while (line[i] && !ft_isspace(line[i]))
		i++;
	return (ft_substr(line, 0, i));
}
