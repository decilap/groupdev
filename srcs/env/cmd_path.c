/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:23:55 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 18:16:01 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Checks if the command is a direct path (contains '/')
 * and verifies its existence.
 *
 * @param cmd The command string.
 * @param err_code Pointer to error code.
 * @return A strdup of the cmd if valid, or NULL otherwise.
 */
static char	*check_direct_path(char *cmd, int *err_code)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		*err_code = 1;
		return (NULL);
	}
	return (NULL);
}

/**
 * @brief Resolves the absolute path of a command based on PATH or direct path.
 *
 * @param cmd  The command to resolve.
 * @param env  The environment variables array.
 * @param err_code Pointer to error code (set if not found).
 * @return A newly allocated string with the full path, or NULL if not found.
 */
char	*get_cmd_path(char *cmd, char **env, int *err_code)
{
	char	*path_env;
	char	**paths;
	char	*result;

	result = check_direct_path(cmd, err_code);
	if (result || *err_code != 0)
		return (result);
	path_env = get_env_value("PATH", env);
	if (!path_env)
	{
		*err_code = 2;
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		*err_code = 2;
		return (NULL);
	}
	result = search_in_path(cmd, paths);
	ft_free_arr(paths);
	if (!result)
		*err_code = 2;
	return (result);
}

/**
 * @brief Resolves the full command path and handles resolution errors.
 *
 * This function delegates expansion and resolution, and handles any
 * error messages if the command cannot be found.
 *
 * @param cmd The command structure.
 * @param shell The shell structure.
 */
void	resolve_cmd_path(t_cmd *cmd, t_shell *shell)
{
	t_token	*expanded;
	int		err_code;

	expanded = expand_and_resolve_path(cmd, shell, &err_code);
	handle_path_errors(cmd, expanded, err_code, shell);
	free_tokens(expanded);
}
