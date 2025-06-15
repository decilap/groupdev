/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 02:41:16 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Met à jour les variables d'environnement `PWD` et `OLDPWD`.
 *
 * Cette fonction stocke l'ancien répertoire courant dans `OLDPWD`,
 * puis récupère le nouveau répertoire courant avec `getcwd()`
 * et le stocke dans `PWD`.
 * En cas d'erreur lors de la récupération du répertoire courant,
 * un message est affiché.
 *
 * @param oldpwd Ancien répertoire courant à enregistrer dans `OLDPWD`.
 * @param shell Structure principale contenant l'environnement.
 */
void	update_pwd_vars(char *oldpwd, t_shell *shell)
{
	char	*cwd;

	if (oldpwd)
		set_env_value("OLDPWD", oldpwd, shell);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		set_env_value("PWD", cwd, shell);
		free(cwd);
	}
	else
		ft_putstr_fd(RED "minishell: cd: error retrieving\
			current directory\n" RESET, STDERR_FILENO);
}

/**
 * @brief Retourne le chemin HOME pour la commande cd sans argument.
 *
 * @param shell Le contexte du shell (pour accéder à l'environnement).
 * @return Une copie du chemin HOME ou NULL si non défini.
 */
static char	*get_cd_home_path(t_shell *shell)
{
	char	*path;

	path = get_env_value("HOME", shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (NULL);
	}
	return (ft_strdup(path));
}

/**
 * @brief Retourne le chemin OLDPWD pour la commande cd -.
 *
 * Affiche la valeur de OLDPWD avant de la retourner.
 *
 * @param shell Le contexte du shell.
 * @return Une copie du chemin OLDPWD ou NULL si non défini.
 */
static char	*get_cd_oldpwd_path(t_shell *shell)
{
	char	*path;

	path = get_env_value("OLDPWD", shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		return (NULL);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	return (ft_strdup(path));
}

/**
 * @brief Résout le chemin cible pour la commande `cd`.
 *
 * @param args Arguments passés à cd.
 * @param shell Structure principale contenant l’environnement.
 * @return Le chemin à utiliser pour chdir, ou NULL en cas d’erreur.
 */
char	*resolve_cd_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
		path = get_cd_home_path(shell);
	else if (ft_strcmp(args[1], "-") == 0)
		path = get_cd_oldpwd_path(shell);
	else
		path = ft_strdup(args[1]);
	if (!path)
		perror("minishell: cd: strdup failed");
	return (path);
}

/**
 * @brief Affiche les variables d'environnement du shell.
 *
 * Imite le comportement de la commande `env` en affichant uniquement les
 * variables contenant le caractère `=` (donc définies avec une valeur).
 *
 * @param shell Structure principale contenant l’environnement.
 * @return 0 toujours (indiquant le succès).
 */
int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	if (args[1])
	{
		ft_putstr_fd(RED "env: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory" RESET, STDERR_FILENO);
		return (127);
	}
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
			ft_putendl_fd(shell->env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
