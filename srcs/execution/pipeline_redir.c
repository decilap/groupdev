/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 20:40:27 by ryoussfi         ###   ########.fr       */
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
bool	redir_prev_fd(int prev_fd)
{
	bool	ret;

	ret = true;
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) < 0)
		{
			perror(RED "minishell: execution: redir_prev_fd" RESET);
			ret = false;
		}
		safe_close(prev_fd);
	}
	return (ret);
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
bool	redir_in(t_cmd *cmd)
{
	bool	ret;

	ret = true;
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) < 0)
		{
			perror(RED "minishell: execution: redir_in" RESET);
			ret = false;
		}
		safe_close(cmd->fd_in);
	}
	return (ret);
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
bool	redir_out(t_cmd *cmd)
{
	bool	ret;

	ret = true;
	if (DEBUG_MODE)
	{
		ft_putstr_fd(RED "[DEBUG] redirecting out to fd=", 2);
		ft_putnbr_fd(cmd->fd_out, 2);
		ft_putstr_fd(";\n" RESET, 2);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) < 0)
		{
			perror(RED "minishell: execution: redir_out" RESET);
			ret = false;
		}
		safe_close(cmd->fd_out);
	}
	return (ret);
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
bool	redir_all(t_cmd *cmd, t_pipe_ctx *ctx)
{
	if (!redir_prev_fd(ctx->prev_fd))
		return (false);
	if (!redir_pipe(ctx->pipefd, ctx->pipe_needed))
		return (false);
	if (!redir_in(cmd))
		return (false);
	if (!redir_out(cmd))
		return (false);
	return (true);
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
bool	redir_pipe(int *pipefd, int pipe_needed)
{
	bool	ret;

	ret = true;
	if (pipe_needed)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror(RED "minishell: execution: redir_pipe" RESET);
			ret = false;
			ret = true;
		}
		safe_close(pipefd[0]);
		safe_close(pipefd[1]);
	}
	return (ret);
}
