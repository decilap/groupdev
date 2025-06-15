/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_local.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:12:32 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 02:58:02 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Met à jour la valeur d'une variable locale existante.
 * @param var Pointeur vers la variable à mettre à jour.
 * @param value Nouvelle valeur à assigner.
 * @return 1 si mise à jour réussie, 0 sinon.
 */
static int	update_local_var(t_var *var, const char *value)
{
	free(var->value);
	var->value = ft_strdup(value);
	if (!var->value)
	{
		perror(RED "minishell: unset - strdup failed" RESET);
		return (0);
	}
	return (1);
}

/**
 * @brief Crée une nouvelle variable locale et l'ajoute à la liste.
 * @param shell Shell principal contenant les variables locales.
 * @param name Nom de la variable.
 * @param value Valeur de la variable.
 */
static void	create_local_var(t_shell *shell, const char *name,
	const char *value)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	if (!var)
	{
		perror(RED "minishell: unset - malloc failed" RESET);
		return ;
	}
	var->key = ft_strdup(name);
	if (!var->key)
	{
		free(var);
		perror(RED "minishell: unset - strdup key failed" RESET);
		return ;
	}
	var->value = ft_strdup(value);
	if (!var->value)
	{
		free(var->key);
		free(var);
		perror(RED "minishell: unset - strdup value failed" RESET);
		return ;
	}
	var->next = shell->locals;
	shell->locals = var;
}

/**
 * @brief Définit ou met à jour une variable locale dans le shell.
 * @param shell Shell principal contenant les variables locales.
 * @param name Nom de la variable.
 * @param value Valeur à attribuer.
 */
void	set_local_var(t_shell *shell, const char *name, const char *value)
{
	t_var	*var;

	var = shell->locals;
	while (var)
	{
		if (ft_strcmp(var->key, name) == 0)
		{
			update_local_var(var, value);
			return ;
		}
		var = var->next;
	}
	create_local_var(shell, name, value);
}

/**
 * @brief Ajoute une variable locale à l'environnement si elle existe, 
 * sinon crée une variable d’environnement vide avec ce nom.
 * 
 * @param shell Le shell dans lequel ajouter la variable à l’environnement.
 * @param name Le nom de la variable à ajouter.
 */
void	add_to_env_from_local(t_shell *shell, const char *name)
{
	t_var	*var;

	var = find_local_var(shell, name);
	if (var)
		set_env_value(name, var->value, shell);
	else
		set_env_value_local(name, shell);
}

/**
 * @brief Retrieve the value of a shell variable (local or environment).
 * 
 * This function first looks for the variable in the shell's local variables.
 * If not found, it falls back to searching in the environment variables.
 * 
 * @param name  The name of the variable to search.
 * @param shell The shell context containing local and environment variables.
 * @return The value of the variable if found, or NULL if not found.
 *         (Note: the returned pointer is not duplicated, it's owned by shell.)
 */
char	*get_shell_var_value(const char *name, t_shell *shell)
{
	char	*env_val;
	t_var	*var;

	var = find_local_var(shell, name);
	if (var && var->value)
		return (var->value);
	env_val = get_env_value(name, shell->env);
	return (env_val);
}
