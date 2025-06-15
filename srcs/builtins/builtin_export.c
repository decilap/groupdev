/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 02:40:40 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Vérifie si une chaîne est un identifiant valide pour export/unset.
 *
 * Un identifiant valide commence par une lettre ou un `_` et contient
 * uniquement des lettres, chiffres ou underscores jusqu’à un éventuel `=`.
 *
 * @param str La chaîne à tester.
 * @return int 1 si valide, 0 sinon.
 */

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 1;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_' && !ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Affiche une ligne exportée au format declare -x.
 *
 * @param var La variable à afficher.
 */
static void	print_export_line(char *var)
{
	char	*value;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	value = ft_strchr(var, '=');
	if (value && value[0] && value[1])
		ft_putstr_fd(var, STDOUT_FILENO);
	else if (value)
	{
		ft_putstr_fd(var, STDOUT_FILENO);
		ft_putstr_fd("\"\"", STDOUT_FILENO);
	}
	else
		ft_putstr_fd(var, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
}

static void	export_print_env(char **env)
{
	int		i;
	char	**sorted_env;

	sorted_env = prepare_sorted_env(env);
	if (!sorted_env)
		return ;
	i = 0;
	while (sorted_env[i])
	{
		print_export_line(sorted_env[i]);
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

/**
 * @brief Gère un argument passé à `export`. Vérifie la validité et applique
 *        l'ajout à l’environnement ou en local.
 *
 * @param arg L’argument à traiter (ex: VAR=valeur).
 * @param shell Le shell courant contenant env et locals.
 * @return int 0 si succès, 1 si identifiant invalide.
 */
static int	handle_export_arg(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd(RED "minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier" RESET, STDERR_FILENO);
		return (1);
	}
	if (ft_strchr(arg, '='))
		add_to_env(shell, arg);
	else
		add_to_env_from_local(shell, arg);
	ft_history_change_size(shell);
	return (0);
}

/**
 * @brief Implémente la commande `export`. Affiche les variables exportées
 *        ou ajoute/modifie les variables selon les arguments.
 *
 * @param args Tableau d’arguments (args[0] = "export", args[1..] = VARs).
 * @param shell Shell courant contenant env et locals.
 * @return int 0 si tout s’est bien passé, 1 si au moins un identifiant invalide.
 */
int	builtin_export(char **args, t_shell *shell)
{
	int	ret;

	ret = 0;
	if (!args[1])
	{
		export_print_env(shell->env);
		return (ret);
	}
	while (*++args)
		ret |= handle_export_arg(*args, shell);
	return (ret);
}
