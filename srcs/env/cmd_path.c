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

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] get_cmd_path(): ENTRY — cmd = \"%s\"\n", cmd);
	*err_code = 0;

	result = check_direct_path(cmd, err_code);
	if (result)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] get_cmd_path(): check_direct_path() -> \"%s\"\n", result);
		return (result);
	}
	if (*err_code != 0)
	{
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] get_cmd_path(): check_direct_path() failed, err_code = %d\n", *err_code);
		return (NULL);
	}

	path_env = get_env_value("PATH", env);
	if (!path_env)
	{
		*err_code = 2;
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] get_cmd_path(): PATH not found in env\n");
		return (NULL);
	}
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] get_cmd_path(): PATH = \"%s\"\n", path_env);

	paths = ft_split(path_env, ':');
	if (!paths)
	{
		*err_code = 2;
		if (DEBUG_MODE)
			fprintf(stderr, "[LOG] get_cmd_path(): ft_split() failed\n");
		return (NULL);
	}

	result = search_in_path(cmd, paths);
	ft_free_arr(paths);

	if (DEBUG_MODE)
	{
		if (result)
			fprintf(stderr, "[LOG] get_cmd_path(): search_in_path() found \"%s\"\n", result);
		else
			fprintf(stderr, "[LOG] get_cmd_path(): command \"%s\" not found in PATH\n", cmd);
	}

	if (!result)
		*err_code = 2;
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] get_cmd_path(): END — returning \"%s\"\n", result ? result : "NULL");
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

	if (DEBUG_MODE)
	{
		fprintf(stderr, "[LOG] resolve_cmd_path(): ENTRY — cmd = %p\n", (void *)cmd);
		fprintf(stderr, "[LOG] resolve_cmd_path(): cmd->cmd_path = %s\n", cmd->cmd_path);
	}
	
	expanded = expand_and_resolve_path(cmd, shell, &err_code);

	if (DEBUG_MODE)
	{
		fprintf(stderr, "[LOG] resolve_cmd_path(): expanded = %p, err_code = %d\n", (void *)expanded, err_code);
		fprintf(stderr, "[LOG] resolve_cmd_path(): calling handle_path_errors()\n");
	}
	handle_path_errors(cmd, expanded, err_code, shell);
    if (expanded)
    {
        // Si t_token contient un `value` alloué (ce qui est le cas pour "__SUBSHELL_X__"),
        // assurez-vous de libérer `expanded_token->value` d'abord.
        if (expanded->value) {
            if (DEBUG_MODE)
                fprintf(stderr, "[LOG] resolve_cmd_path(): Freeing expanded_token->value: \"%s\"\n", expanded->value);
            free(expanded->value);
            expanded->value = NULL;
        }
        if (DEBUG_MODE)
            fprintf(stderr, "[LOG] resolve_cmd_path(): Freeing expanded_token: %p\n", (void*)expanded);
        free(expanded);
        expanded = NULL; // Mettre à NULL après libération
    }
	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] resolve_cmd_path(): returned from handle_path_errors(), calling free_tokens()\n");

	free_tokens(expanded);

	if (DEBUG_MODE)
		fprintf(stderr, "[LOG] resolve_cmd_path(): END — cmd = %p\n", (void *)cmd);
}

