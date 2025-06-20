/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 02:40:54 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Affiche le répertoire de travail actuel.
 *
 * Cette fonction utilise `getcwd` pour obtenir le chemin absolu
 * du répertoire courant
 * et l'affiche sur la sortie standard. Si `getcwd` échoue, elle affiche
 * une erreur avec `perror`.
 *
 * @return int Toujours 0 (indiquant un succès même en cas d'échec de `getcwd`).
 */
int	builtin_pwd(char **args)
{
	char	*cwd;

	if (args[1])
	{
		ft_putendl_fd(RED "minishell: pwd: too many arguments\
			" RESET, STDERR_FILENO);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		free(cwd);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
	return (0);
}
