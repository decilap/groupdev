/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/23 13:49:02 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Executes an external command using execve, handling common errors.
 *
 * If execve fails, attempts to run the file as a script using /bin/sh,
 * or prints a detailed error message and exits with an appropriate code.
 *
 * @param cmd   The command to execute.
 * @param shell The shell context (for environment).
 */
static void	handle_execve(t_cmd *cmd, t_shell *shell)
{
	if (execve(cmd->cmd_path, cmd->args, shell->env) == -1)
	{
		if (errno == ENOEXEC)
			execve("/bin/sh", (char *[]){"/bin/sh", cmd->cmd_path, NULL},
				shell->env);
		if (errno == EACCES)
		{
			ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_path, STDERR_FILENO);
			ft_putendl_fd(": Permission denied" RESET, STDERR_FILENO);
			exit(126);
		}
		if (errno == ENOENT)
		{
			ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_path, STDERR_FILENO);
			ft_putendl_fd(": No such file or directory" RESET, STDERR_FILENO);
			exit(127);
		}
		exit_error("execve failed");
	}
}

/**
 * @brief Handles common execution errors before execve.
 *
 * Checks if the command is missing or if the command path is a directory,
 * printing the appropriate error message and exiting with the correct code.
 *
 * @param cmd The command to check.
 * @param sb  Pointer to a stat structure for checking file type.
 */
static void	handle_exec_err(t_cmd *cmd, struct stat *sb)
{
	if (stat(cmd->cmd_path, sb) == 0 && S_ISDIR(sb->st_mode))
	{
		ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd_path, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n" RESET, STDERR_FILENO);
		exit(126);
	}
}

/**
 * @brief Execute a builtin command without any redirection handling.
 *
 * This function simply delegates directly to run_builtin().
 *
 * @param cmd The command structure.
 * @param shell The shell state.
 * @return The return value of the builtin execution.
 */
static int	execute_builtin_no_redir(t_cmd *cmd, t_shell *shell)
{
	return (run_builtin(cmd, shell));//la
}

/**
 * @brief Main entry point for executing a command in the child process.
 *
 * Sets up redirections, handles signals, and executes either a builtin
 * or an external command.
 * Exits the child process with the appropriate status.
 *
 * @param cmd   The command to execute.
 * @param shell The shell context (for environment, builtins, etc.).
 * @param ctx   The pipe/redirection context.
 * @param sb    Pointer to a stat structure (for file/dir checks).
 */

void	child_process(t_cmd *cmd, t_shell *shell, t_pipe_ctx *ctx,
	struct stat *sb)
{
	int	status;

	signal(SIGQUIT, NULL);
	redir_all(cmd, ctx);
	if (!cmd->args || !cmd->args[0])
		exit(127);
	if (is_builtin(cmd))
	{
		status = execute_builtin_no_redir(cmd, shell);// la , t_pipe_ctx *pipe_ctx
		exit(status);
	}
	if (!cmd->cmd_path)
		exit(127);
	handle_exec_err(cmd, sb);
	handle_execve(cmd, shell);
	exit(126);
}
