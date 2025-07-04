/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/23 13:54:27 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Affiche l'historique des commandes stockées dans le shell.
 *
 * Cette fonction utilise ft_print_history pour afficher toutes les commandes 
 * de l'historique du shell avec un index.
 *
 * @param shell Pointeur vers la structure shell contenant l'historique.
 * @return 0 en cas de succès, 1 si une erreur est détectée.
 */
static int	builtin_history(t_shell *shell)
{
	t_hist	*current;
	size_t	i;

	i = 1;
	if (!shell)
		return (1);
	if (!shell->history || !shell->history->cmd)
		return (0);
	current = shell->history;
	ft_print_history(&current, &i);
	return (0);
}

/**
 * @brief Exécute la commande built-in appropriée selon le nom de la commande.
 *
 * Cette fonction redirige vers l’implémentation de la commande interne
 * correspondante comme `cd`, `pwd`, `echo`, etc.
 *
 * @param cmd Structure contenant la commande à exécuter.
 * @param shell Structure contenant les données globales du shell.
 * @return Code de retour de la commande exécutée.
 */
int	run_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	else if (ft_strcmp(cmd->args[0], "history") == 0)
		return (builtin_history(shell));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd->args, shell));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(cmd->args));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd->args, shell));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd->args, shell));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(cmd->args, shell));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd->args, shell));
	return (1);
}

/**
 * @brief Gère la redirection de l'entrée/sortie pour une commande built-in
 * et l’exécute.
 *
 * Cette fonction sauvegarde les descripteurs standards, applique les
 * redirections de la commande,
 * exécute le built-in, puis restaure les descripteurs standards d’origine.
 *
 * @param cmd Structure contenant la commande à exécuter.
 * @param shell Structure globale du shell.
 * @return Code de retour de la commande exécutée.
 */
int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	int	ret;

	shell->saved_in = dup(STDIN_FILENO);
	shell->saved_out = dup(STDOUT_FILENO);
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	ret = run_builtin(cmd, shell);
	dup2(shell->saved_in, STDIN_FILENO);
	dup2(shell->saved_out, STDOUT_FILENO);
	close(shell->saved_in);
	close(shell->saved_out);
	return (ret);
}
