/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/23 13:58:43 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Valide les arguments passés à la commande `exit`.
 *
 * Cette fonction vérifie les cas suivants :
 * - Aucun argument : utilise le code de sortie actuel du shell.
 * - Argument non numérique : affiche une erreur et définit
 * un code de sortie 255.
 * - Trop d’arguments : affiche une erreur et retourne une erreur sans quitter.
 * - Argument valide : convertit en entier le code de sortie.
 *
 * @param args Les arguments de la commande `exit`.
 * @param shell Pointeur vers la structure du shell.
 * @param exit_code Pointeur vers une variable qui recevra le code de sortie.
 * @return 0 si tout est valide, 1 s’il y a trop d’arguments.
 */
static int	validate_exit_args(char **args, t_shell *shell, long *exit_code)
{
	if (!args[1])
	{
		*exit_code = shell->exit_status;
		return (0);
	}
	if (!ft_isnumber(args[1]))
	{
		ft_putstr_fd(RED "minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required" RESET, STDERR_FILENO);
		*exit_code = 2;
		return (0);
	}
	if (args[2])
	{
		ft_putstr_fd(RED "minishell: exit: too many arguments\
			\n" RESET, STDERR_FILENO);
		shell->exit_status = 1;
		return (1);
	}
	*exit_code = ft_atoi(args[1]);
	return (0);
}

/**
 * @brief Implémente la commande `exit` pour quitter le shell.
 *
 * Valide les arguments et effectue une sortie propre avec le bon code de sortie.
 * Affiche "exit" sur stdout comme dans Bash.
 *
 * @param args Les arguments de la commande `exit`.
 * @param shell Pointeur vers la structure du shell.
 * @return 1 en cas d’erreur (trop d’arguments), 0 sinon.
 */
int	builtin_exit(char **args, t_shell *shell)
{
	long	exit_code;
	int		err;

	exit_code = 0;
	ft_putstr_fd("exit\n", 1);
	err = validate_exit_args(args, shell, &exit_code);
	if (err)
		return (1);
	shell->exit_status = (unsigned char)exit_code;
	free_groups(shell->groups);
	dup2(shell->saved_in, STDIN_FILENO);
	dup2(shell->saved_out, STDOUT_FILENO);
	close(shell->saved_in);
	close(shell->saved_out);
	free(shell->line);
	ft_free_arr(shell->lines);
	clean_exit(shell);
	return (404);
}
