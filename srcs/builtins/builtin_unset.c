/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 23:16:17 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Supprime des variables d’environnement et locales données en arguments.
 *
 * Pour chaque nom de variable passé en argument, cette fonction supprime
 * la variable de l’environnement (`env`) et des variables locales (`locals`)
 * si elle existe. Elle ignore les identifiants invalides.
 *
 * @param args Tableau d’arguments (args[0] = "unset",
 * args[1..] = noms de variables).
 * @param shell Structure du shell contenant les environnements
 * à modifier.
 * @return int 0 (aucune erreur n’est remontée même si un identifiant est
 * invalide).
 */
int	builtin_unset(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd(RED "minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier" RESET, STDERR_FILENO);
			ret = 1;
		}
		else
			remove_env_var(args[i], shell);
		i++;
	}
	return (ret);
}
