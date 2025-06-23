/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 13:46:04 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# define SYNTAX_ERROR_EXIT_CODE 258

/**
 * @brief I/O context for managing pipes and previous
 * file descriptors during execution.
 *
 * Used for tracking file descriptors and pipe status between commands.
 */
typedef struct s_io_ctx
{
	int	prev_fd;
	int	*pipefd;
	int	pipe_needed;
}	t_io_ctx;

/**
 * @brief Context for managing subshell execution (internal shell logic).
 *
 * Stores pipe and file descriptor information, subshell line, and nesting depth.
 */
typedef struct s_subsh_ctx
{
	int		prev_fd;
	int		pipe_needed;
	int		depth;
	int		*pipefd;
	char	*sub_line;
}	t_subsh_ctx;

/**
 * @brief Pipeline execution context for managing process, pipes, and status.
 *
 * Stores file descriptors for previous/next commands, current pipe,
 * pointer to process IDs, and stat struct for file/dir info.
 */
typedef struct s_pipe_ctx
{
	int			prev_fd;
	int			pipefd[2];
	int			pipe_needed;
	pid_t		*pid;
	struct stat	*sb;
}	t_pipe_ctx;

/**
 * @brief Context for managing an individual subshell in execution.
 *
 * Stores previous file descriptor, pipe info, and the subshell's command line.
 */
typedef struct s_subshell_ctx
{
	int		prev_fd;
	int		pipe_needed;
	int		*pipefd;
	char	*sub_line;
}	t_subshell_ctx;

/**
 * @brief High-level context for executing an entire pipeline.
 *
 * Tracks current pipeline position, depth (for nested subshells),
 * all process IDs in the pipeline, and pointer to the pipe context.
 */
typedef struct s_pipeline_ctx
{
	int			*i;
	int			depth;
	pid_t		*pids;
	t_pipe_ctx	*pipe_ctx;
}	t_pipeline_ctx;

int		execute_pipeline_group(t_cmd **cmd_ptr, t_shell *shell);
int		handle_pipeline_step(t_cmd *cmd, t_shell *shell, t_pipeline_ctx *ctx);
int		execute_logical_groups(t_group *groups, t_shell *shell);
int		exec_pipeline_loop(t_cmd *cmd, t_shell *shell, pid_t *pids);
int		execute_with_logical(t_cmd *cmd, t_shell *shell);
int		is_subshell(t_cmd *cmd);
int		handle_subshell(t_cmd *cmd, t_shell *shell, t_subsh_ctx *ctx);
int		waitpids(pid_t *pids, int count, int final_status);
void	child_process(t_cmd *cmd, t_shell *shell, t_pipe_ctx *ctx,
			struct stat *sb);
void	redir_all(t_cmd *cmd, t_pipe_ctx *ctx);
void	pipe_prepare(t_pipe_ctx *ctx, t_cmd *cmd, t_shell *shell);
void	print_subshell_syntax_error(const char *line);
void	update_ctx_after_cmd(t_pipe_ctx *ctx);
void	child_process(t_cmd *cmd, t_shell *shell, t_pipe_ctx *ct,
			struct stat *sb);
void	expand_single_arg(t_cmd *cmd, t_shell *shell, int i);
void	expand_cmd_args(t_cmd *cmd, t_shell *shell);
void	init_pipe_ctx(t_pipe_ctx *ctx);
void	clear_subshell_table(void);
void	redir_prev_fd(int prev_fd);
void	redir_pipe(int *pipefd, int pipe_needed);
void	redir_in(t_cmd *cmd);
void	redir_out(t_cmd *cmd);
void	subshell_child(t_cmd *cmd, t_shell *shell, t_subsh_ctx *ctx);

#endif
