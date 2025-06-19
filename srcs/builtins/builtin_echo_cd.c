/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 20:18:45 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Handles the built-in echo command.
 *
 * Prints its arguments separated by spaces, followed by a newline
 * unless the `-n` flag is set.
 *
 * @param args Null-terminated array of strings representing command-line
 * arguments.
 * @return Always returns 0 (success).
 */
int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	i = parse_echo_flags(args, &newline);
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1] && args[i + 1][0] != '\0')
			safe_write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		safe_write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/**
 * @brief Attempts to change the current directory to the given path.
 *
 * If chdir fails, prints an error message to stderr and frees
 * the old working directory.
 *
 * @param path Path to the directory to change to.
 * @param oldpwd Previously stored working directory, freed on error.
 * @return 0 on success, 1 on failure.
 */
static int	try_chdir(char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd(RED "minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n" RESET, STDERR_FILENO);
		return (1);
	}
	return (0);
}

/**
 * @brief Prépare les chemins nécessaires avant d'exécuter cd.
 *
 * Récupère le répertoire courant (OLDPWD) et résout le chemin cible.
 *
 * @param args Arguments passés à cd.
 * @param shell Contexte du shell.
 * @param oldpwd Pointeur de sortie vers oldpwd alloué.
 * @param path Pointeur de sortie vers le chemin cible alloué.
 * @return 0 en cas de succès, 1 en cas d'erreur.
 */
static int	prepare_cd_paths(char **args, t_shell *shell, char **oldpwd,
	char **path)
{
	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
		*oldpwd = ft_strdup("");
	*path = resolve_cd_path(args, shell);
	if (!*path)
	{
		free(*oldpwd);
		return (1);
	}
	return (0);
}

/**
 * @brief Handles the built-in cd command.
 *
 * @param args Arguments of the cd command.
 * @param shell Shell state.
 * @return 0 on success, 1 on failure.
 */
int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*oldpwd;

	if (args[1] && args[2])
	{
		ft_putendl_fd(RED "minishell: cd: too many arguments\
			" RESET, STDERR_FILENO);
		return (1);
	}
	if (prepare_cd_paths(args, shell, &oldpwd, &path))
		return (1);
	if (try_chdir(path))
	{
		free(oldpwd);
		free(path);
		return (1);
	}
	update_pwd_vars(oldpwd, shell);
	free(oldpwd);
	free(path);
	return (0);
}
