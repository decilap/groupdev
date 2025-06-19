/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 20:18:34 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Initializes a pipe context for pipeline execution.
 *
 * Sets file descriptors and flags to default unused values.
 *
 * @param ctx Pointer to the pipe context to initialize.
 */
void	init_pipe_ctx(t_pipe_ctx *ctx)
{
	ctx->prev_fd = -1;
	ctx->pipe_needed = 0;
	ctx->pipefd[0] = -1;
	ctx->pipefd[1] = -1;
}

/**
 * @brief Updates the pipe context after executing a command in a pipeline.
 *
 * Closes any previous input file descriptor, closes the write end
 * of the current pipe
 * if needed, and moves the read end to prev_fd for the next command.
 *
 * @param ctx Pointer to the pipe context.
 */
void	update_ctx_after_cmd(t_pipe_ctx *ctx)
{
	if (ctx->prev_fd != -1)
		safe_close(ctx->prev_fd);
	if (ctx->pipe_needed)
		safe_close(ctx->pipefd[1]);
	if (ctx->pipe_needed)
		ctx->prev_fd = ctx->pipefd[0];
	else
		ctx->prev_fd = -1;
}

/**
 * @brief Waits for all child processes in a pipeline and
 * collects the final status.
 *
 * Handles normal exit and signals, displays core dump message for SIGQUIT,
 * and ensures the final status reflects the last process in the pipeline.
 *
 * @param pids         Array of process IDs to wait for.
 * @param count        Number of processes in the pipeline.
 * @param final_status Initial/final status to update.
 * @return The final status (exit code or signal) of the last pipeline process.
 */
int	waitpids(pid_t *pids, int count, int final_status)
{
	int	k;
	int	sig;
	int	wstatus;

	k = 0;
	wstatus = 0;
	while (k < count)
	{
		waitpid(pids[k], &wstatus, 0);
		if (k == count - 1)
		{
			sig = WTERMSIG(wstatus);
			if (WIFEXITED(wstatus))
				final_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				final_status = 128 + sig;
			if (sig == SIGQUIT)
				ft_putendl_fd(RED "Quit (core dumped)" RESET, STDERR_FILENO);
		}
		k++;
	}
	if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT)
		safe_write(STDOUT_FILENO, "\n", 1);
	return (final_status);
}

/**
 * @brief Prepares a pipe for the current command if a pipe is needed.
 *
 * Sets the pipe_needed flag and creates the pipe file
 * descriptors if appropriate.
 * Exits with an error if pipe creation fails.
 *
 * @param ctx Pipe context to update.
 * @param cmd The current command (checked for pipe necessity).
 */
bool	pipe_prepare(t_pipe_ctx *ctx, t_cmd *cmd)
{
	if (cmd->next && cmd->next_type == TOKEN_PIPE)
		ctx->pipe_needed = 1;
	else
		ctx->pipe_needed = 0;
	if (ctx->pipe_needed)
	{
		if (pipe(ctx->pipefd) == -1)
		{
			perror(RED "minishell: execution: pipe_prepare: pipe error" RESET);
			return (false);
		}
	}
	return (true);
}
