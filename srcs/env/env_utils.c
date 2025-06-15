/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:40:19 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 23:36:22 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Retrieves the value of an environment variable by its name.
 *
 * @param name The name of the environment variable.
 * @param env The environment array.
 * @return The value of the environment variable or NULL if not found.
 */
char	*get_env_value(const char *name, char **env)
{
	int		i;
	char	*eq;
	size_t	name_len;
	size_t	key_len;

	i = 0;
	name_len = ft_strlen(name);
	while (env && env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			key_len = eq - env[i];
			if (key_len == name_len
				&& ft_strncmp(env[i], name, name_len) == 0)
				return (eq + 1);
		}
		i++;
	}
	return (NULL);
}

/**
 * @brief Copies an environment array. Exits the program if allocation fails.
 *
 * @param envp The source environment array.
 * @return A new copy of the environment array.
 */
char	**copy_env(char **envp)
{
	int		i;
	int		env_count;
	char	**new_env;

	env_count = 0;
	while (envp[env_count])
		env_count++;
	new_env = safe_malloc(sizeof(char *) * (env_count + 1));
	i = 0;
	while (i < env_count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (i > 0)
				free(new_env[--i]);
			free(new_env);
			exit_error(RED "minishell: unset" RESET);
			return (NULL);
		}
		i++;
	}
	new_env[env_count] = NULL;
	return (new_env);
}

/**
 * @brief Construit le chemin complet d'un binaire à tester.
 *
 * @param dir Le dossier courant dans PATH.
 * @param cmd La commande recherchée.
 * @return Le chemin complet (à free), ou NULL si erreur malloc.
 */
static char	*build_full_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
	{
		perror(RED "minishell: malloc failed (tmp)" RESET);
		return (NULL);
	}
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full)
		perror(RED "minishell: malloc failed (full)" RESET);
	return (full);
}

/**
 * @brief Recherche l'exécutable dans les chemins PATH.
 *
 * @param cmd La commande à chercher.
 * @param paths Tableau des répertoires PATH.
 * @return Chemin complet trouvé (à free), ou NULL si non trouvé.
 */
char	*search_in_path(char *cmd, char **paths)
{
	int		i;
	char	*full;

	if (!cmd || !*cmd)
		return (NULL);
	i = 0;
	while (paths && paths[i])
	{
		full = build_full_path(paths[i], cmd);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

/**
 * @brief Removes an environment variable from the shell structure.
 *
 * @param name The name of the variable to remove.
 * @param shell The structure holding the environment.
 */
void	remove_env_var(const char *name, t_shell *sh)
{
	int		i;
	int		j;
	size_t	len;
	char	**new_env;

	i = 0;
	j = 0;
	len = ft_strlen(name);
	while (sh->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (perror("minishell: 5 unset"));
	i = -1;
	while (sh->env[++i])
	{
		if (ft_strncmp(sh->env[i], name, len) == 0
			&& (sh->env[i][len] == '=' || sh->env[i][len] == '\0'))
			free(sh->env[i]);
		else
			new_env[j++] = sh->env[i];
	}
	new_env[j] = NULL;
	free(sh->env);
	sh->env = new_env;
}
