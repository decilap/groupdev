/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 00:39:04 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Redirects the previous pipe's read end to standard input (stdin).
 *
 * Used in pipelines to connect the previous command's output
 * to the current command's input.
 * Closes the original file descriptor after duplication.
 *
 * @param prev_fd File descriptor to duplicate (typically the previous
 * pipe's read end).
 */
void	redir_prev_fd(int prev_fd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
}

/**
 * @brief Redirects the command's input file descriptor to
 * standard input (stdin).
 *
 * If the command uses a custom input (from redirection or pipe), duplicates
 * it and closes the original.
 *
 * @param cmd The command containing the input file descriptor.
 */
void	redir_in(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
}

/**
 * @brief Redirects the command's output file descriptor
 * to standard output (stdout).
 *
 * If the command uses a custom output (from redirection or pipe),
 * duplicates it and closes the original.
 *
 * @param cmd The command containing the output file descriptor.
 */
void	redir_out(t_cmd *cmd)
{
	if (DEBUG_MODE)
		fprintf(stderr, "[DEBUG] redirecting out to fd=%d\n", cmd->fd_out);
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
}

/**
 * @brief Performs all required redirections for a command in a pipeline.
 *
 * Handles previous pipe input, current pipe output, and any command-specific
 * input/output redirections (e.g., from < or > operators).
 *
 * @param cmd The command to set up redirections for.
 * @param ctx The pipeline context (provides previous and current pipe FDs).
 */
void	redir_all(t_cmd *cmd, t_pipe_ctx *ctx)
{
	redir_prev_fd(ctx->prev_fd);
	redir_pipe(ctx->pipefd, ctx->pipe_needed);
	redir_in(cmd);
	redir_out(cmd);
}

/**
 * @brief Redirects the write end of a pipe to standard output
 * (stdout), if needed.
 *
 * Used for commands in the middle or start of a pipeline.
 * Closes only the write end after duplication; the read end
 * will be used by the next process.
 *
 * @param pipefd      The array containing the pipe file descriptors.
 * @param pipe_needed Flag indicating if a pipe is required.
 */
void	redir_pipe(int *pipefd, int pipe_needed)
{
	if (pipe_needed)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
}
