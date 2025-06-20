/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:20:54 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 23:35:17 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Updates an existing environment variable.
 *
 * @param name Variable name.
 * @param new_var New variable string "NAME=VALUE".
 * @param shell Shell structure holding env.
 * @param len Length of name.
 * @return 1 if updated, 0 if not found.
 */
static int	update_existing_env(const char *name, char *new_var,
	t_shell *shell, size_t len)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
		{
			free(shell->env[i]);
			shell->env[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Appends a new variable to the environment.
 *
 * @param new_var New variable string.
 * @param shell Shell structure.
 * @param count Current environment size.
 */
static void	append_new_env(char *new_var, t_shell *shell, int count)
{
	int		j;
	char	**new_env;

	j = 0;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (perror(RED "minishell: 4 export" RESET));
	while (j < count)
	{
		new_env[j] = shell->env[j];
		j++;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
}

/**
 * @brief Sets or updates an environment variable in shell.
 *
 * @param name Variable name.
 * @param value Variable value.
 * @param shell Shell structure.
 */
void	set_env_value(const char *name, const char *value, t_shell *shell)
{
	int		i;
	size_t	len;
	char	*new_var;

	len = ft_strlen(name);
	new_var = ft_strjoin_3(name, "=", value);
	if (!new_var)
		return (perror(RED "minishell: 3 export" RESET));
	i = 0;
	while (shell->env[i])
		i++;
	if (update_existing_env(name, new_var, shell, len))
		return ;
	append_new_env(new_var, shell, i);
}

/**
 * @brief Sets or updates an environment variable in shell.
 *
 * @param name Variable name.
 * @param value Variable value.
 * @param shell Shell structure.
 */
void	set_env_value_local(const char *name, t_shell *shell)
{
	int		i;
	size_t	len;
	char	*new_var;

	len = ft_strlen(name);
	new_var = ft_strdup(name);
	if (!new_var)
		return (perror(RED "minishell: 3 export" RESET));
	i = 0;
	while (shell->env[i])
		i++;
	if (update_existing_env(name, new_var, shell, len))
		return ;
	append_new_env(new_var, shell, i);
}

/**
 * @brief Adds or updates an environment variable in the shell structure.
 *
 * @param name The variable name.
 * @param value The value to assign.
 * @param shell The structure holding the environment.
 */
void	add_to_env(t_shell *shell, char *args)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	int		name_len;

	equal_sign = ft_strchr(args, '=');
	if (!equal_sign)
		return (perror(RED "minishell: 0 export" RESET));
	name_len = equal_sign - args;
	name = ft_strndup(args, name_len);
	if (!name)
		return (perror(RED "minishell: 1 export" RESET));
	value = ft_strdup(equal_sign + 1);
	if (!value)
	{
		free(name);
		return (perror(RED "minishell: 2 export" RESET));
	}
	set_env_value(name, value, shell);
	free(name);
	free(value);
}
