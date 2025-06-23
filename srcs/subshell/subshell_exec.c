/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:30:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 02:12:38 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Sets up file descriptors and environment for the subshell.
 *
 * Sets up input/output redirections and creates a new copy of the environment,
 * isolating the subshell from the parent shell's environment.
 *
 * @param cmd        The command for the subshell.
 * @param shell      The main shell context.
 * @param ctx        The subshell context (fds, pipes).
 * @param env_backup Pointer to store a backup of the original environment.
 */
static void	subshell_prepare_fds_and_env(t_cmd *cmd, t_shell *shell,
	t_subsh_ctx *ctx, char ***env_backup)
{
	signal(SIGQUIT, SIG_DFL);
	redir_prev_fd(ctx->prev_fd);
	redir_pipe(ctx->pipefd, ctx->pipe_needed);
	redir_in(cmd);
	redir_out(cmd);
	*env_backup = shell->env;
	shell->env = copy_env(shell, shell->env);
}

/**
 * @brief Prepares the subshell context and checks
 * for syntax errors before execution.
 *
 * Verifies maximum subshell nesting, initializes
 * file descriptors and environment,
 * and validates the subshell line for syntax. Exits on error.
 *
 * @param cmd        The command representing the subshell.
 * @param shell      The main shell context.
 * @param ctx        The subshell context (fds, pipe info, line, depth).
 * @param env_backup Pointer to save a backup of the environment.
 */
static void	subshell_prepare_and_check(t_cmd *cmd, t_shell *shell,
	t_subsh_ctx *ctx, char ***env_backup)
{
	if (ctx->depth > MAX_SUBSHELLS)
	{
		ft_putstr_fd(RED "minishell: maximum subshell nesting\
			level reached\n" RESET, STDERR_FILENO);
		exit(SYNTAX_ERROR_EXIT_CODE);
	}
	*env_backup = NULL;
	if (ctx->pipe_needed)
	{
		close(ctx->pipefd[0]);
		close(ctx->pipefd[1]);
	}
	subshell_prepare_fds_and_env(cmd, shell, ctx, env_backup);
	if (!ctx->sub_line || is_blank(ctx->sub_line)
		|| !check_parentheses(ctx->sub_line))
	{
		subshell_print_syntax_error(ctx->sub_line);
		subshell_clean_exit(shell, *env_backup, SYNTAX_ERROR_EXIT_CODE);
	}
}

/**
 * @brief Adds a subshell command group to the group linked list.
 *
 * Allocates and initializes a new t_group node with the given command chain.
 * Sets its operator type, updates the linked list, and returns the pointer to
 * the next command to process.
 *
 * @param cmds        The command structure corresponding to the subshell.
 * @param head        Pointer to the head pointer of the group list.
 * @param curr_group  Pointer to the current group pointer
 * (updated to new group).
 * @return The next t_cmd to process, or NULL.
 */
t_cmd	*group_add_subshell(t_cmd *cmds, t_group **head,
	t_group **curr_group, t_shell *shell)
{
	t_group	*new_group;
	t_cmd	*next;

	new_group = create_group(cmds, shell);
	if (!new_group)
		return (NULL);
	new_group->next_op = cmds->next_type;
	next = cmds->next;
	cmds->next = NULL;
	if (!*head)
		*head = new_group;
	else
		(*curr_group)->next = new_group;
	*curr_group = new_group;
	return (next);
}

/**
 * @brief Entry point for executing a subshell in the child process.
 *
 * Prepares file descriptors, environment, parses the subshell command,
 * checks for errors, executes the logic, and exits with the appropriate status.
 *
 * @param cmd   The command struct for the subshell.
 * @param shell The main shell context.
 * @param ctx   The subshell context (fds, pipe info, line, depth).
 */
void	subshell_child(t_cmd *cmd, t_shell *shell, t_subsh_ctx *ctx)
{
	int		sub_status;
	char	**env_backup;
	t_cmd	*sub_cmds;

	subshell_prepare_and_check(cmd, shell, ctx, &env_backup);
	redir_prev_fd(ctx->prev_fd);
	redir_pipe(ctx->pipefd, ctx->pipe_needed);
	sub_cmds = parse_input(ctx->sub_line, shell, 0);
	if (!sub_cmds)
		subshell_clean_exit(shell, env_backup, SYNTAX_ERROR_EXIT_CODE);
	sub_status = execute_with_logical(sub_cmds, shell);
	subshell_clean_exit(shell, env_backup, sub_status);
}

/**
 * @brief Handles a pipeline step involving a subshell command.
 *
 * Prepares the subshell context (pipes, fds, command string, nesting depth),
 * invokes the handler for subshell execution, and updates the pipeline context.
 *
 * @param cmd   The current command (should be a subshell).
 * @param shell The main shell context.
 * @param ctx   The pipeline execution context.
 * @return      The exit status of the subshell step.
 */
int	step_subshell(t_cmd *cmd, t_shell *shell, t_pipeline_ctx *ctx)
{
	int			status;
	t_subsh_ctx	subctx;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	pipe_prepare(ctx->pipe_ctx, cmd, shell);
	subctx.prev_fd = ctx->pipe_ctx->prev_fd;
	subctx.pipefd = ctx->pipe_ctx->pipefd;
	subctx.pipe_needed = (cmd->next_type == TOKEN_PIPE);
	subctx.sub_line = get_subshell_command(cmd->args[0]);
	subctx.depth = ctx->depth + 1;
	if (!subctx.sub_line)
		return (1);
	status = handle_subshell(cmd, shell, &subctx);
	update_ctx_after_cmd(ctx->pipe_ctx);
	return (status);
}
