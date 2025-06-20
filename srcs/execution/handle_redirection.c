/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 00:46:57 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Prints a formatted error message for a redirection failure.
 *
 * Outputs: minishell: <file>: <strerror(errno)>
 *
 * @param file The filename that caused the error.
 */
static void	print_redir_error(char *file)
{
	ft_putstr_fd(RED "minishell: ", STDERR_FILENO);
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd(": " RESET, STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

/**
 * @brief Handles input redirection ('<') for a command.
 *
 * Advances to the next token, expands a possible tilde in the filename,
 * attempts to open the file for reading, and sets curr->fd_in.
 * On failure, prints an error and sets exit_status.
 *
 * @param current Pointer to the current token pointer (will be advanced).
 * @param curr    The command being constructed.
 * @param shell   The shell context (for tilde expansion and error reporting).
 */
static void	handle_redir_in(t_token **current, t_cmd *curr, t_shell *shell)
{
	int		fd;
	char	*filename;

	fd = -1;
	*current = (*current)->next;
	if (*current && (*current)->value)
	{
		filename = expand_tilde_filename((*current)->value, shell);
		(*current)->value = filename;
		fd = open(filename, O_RDONLY);
	}
	if (fd == -1)
	{
		print_redir_error((*current)->value);
		curr->fd_in = -1;
		shell->exit_status = 1;
	}
	else
		curr->fd_in = fd;
}

/**
 * @brief Handles output redirection ('>') for a command.
 *
 * Advances to the next token, expands a possible tilde in the filename,
 * opens the file for writing/truncating, and sets curr->fd_out.
 * On failure, prints an error and sets exit_status.
 *
 * @param current Pointer to the current token pointer (will be advanced).
 * @param curr    The command being constructed.
 * @param shell   The shell context.
 */
static void	handle_redir_out(t_token **current, t_cmd *curr, t_shell *shell)
{
	int		fd;
	char	*filename;

	fd = -1;
	*current = (*current)->next;
	if (*current && (*current)->value)
	{
		filename = expand_tilde_filename((*current)->value, shell);
		(*current)->value = filename;
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (fd == -1)
	{
		print_redir_error((*current)->value);
		curr->fd_out = -1;
		shell->exit_status = 1;
	}
	else
		curr->fd_out = fd;
}

/**
 * @brief Handles append output redirection ('>>') for a command.
 *
 * Advances to the next token, expands a possible tilde in the filename,
 * opens the file for writing/appending, and sets curr->fd_out.
 * On failure, prints an error and sets exit_status.
 *
 * @param current Pointer to the current token pointer (will be advanced).
 * @param curr    The command being constructed.
 * @param shell   The shell context.
 */
static void	handle_redir_append(t_token **current, t_cmd *curr, t_shell *shell)
{
	int		fd;
	char	*filename;

	fd = -1;
	*current = (*current)->next;
	if (*current && (*current)->value)
	{
		filename = expand_tilde_filename((*current)->value, shell);
		(*current)->value = filename;
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if (fd == -1)
	{
		print_redir_error((*current)->value);
		curr->fd_out = -1;
		shell->exit_status = 1;
	}
	else
		curr->fd_out = fd;
}

/**
 * @brief Handles any redirection token (<, >, >>) in the parsing process.
 *
 * Dispatches to the appropriate handler based on the token type,
 * and advances the token pointer as needed.
 *
 * @param tok   Pointer to the token pointer to process (will be advanced).
 * @param curr  The command being constructed.
 * @param shell The shell context.
 */
void	handle_redirection_token(t_token **tok, t_cmd *curr, t_shell *shell)
{
	t_token	*current;

	if (!*tok)
		return ;
	current = *tok;
	if (current->type == TOKEN_REDIR_IN && current->next)
		handle_redir_in(&current, curr, shell);
	else if (current->type == TOKEN_REDIR_OUT && current->next)
		handle_redir_out(&current, curr, shell);
	else if (current->type == TOKEN_APPEND && current->next)
		handle_redir_append(&current, curr, shell);
	else
		return ;
	*tok = current;
}
