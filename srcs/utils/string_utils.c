/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:05:47 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/21 18:17:06 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Concatenates three strings into a newly allocated string.
 *  Exits the program if allocation fails.
 * 
 * @param s1 The first string.
 * @param s2 The second string.
 * @param s3 The third string.
 * @return A newly allocated string containing the concatenated result.
 */
char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	res = (char *)malloc(len1 + len2 + len3 + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	ft_memcpy(res + len1 + len2, s3, len3);
	res[len1 + len2 + len3] = '\0';
	return (res);
}

/**
 * @brief Safely closes a file descriptor and handles any errors.
 *
 * This function attempts to close the given file descriptor. If the operation
 * fails, it prints an error message and returns false.
 *
 * @param fd The file descriptor to close.
 * @return true if the file descriptor was successfully closed, false otherwise.
 */
bool	safe_close(int fd)
{
	if (close(fd) < 0)
	{
		ft_putstr_fd(RED "minishell: failed to close fd=", STDERR_FILENO);
		ft_putnbr_fd(fd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putendl_fd(RESET, STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	is_export_builtin(t_cmd *cmd)
{
	return (cmd->args && cmd->args[0]
		&& ft_strcmp(cmd->args[0], "export") == 0);
}

/**
 * @brief Safely writes data to a file descriptor and handles errors.
 *
 * This function attempts to write `n` bytes from the buffer `buf` to the file 
 * descriptor `fd`. If the write operation fails, an error message is printed
 * to standard output with additional details about the failure.
 *
 * @param fd The file descriptor to which data will be written.
 * @param buf A pointer to the buffer containing the data to be written.
 * @param n The number of bytes to write from the buffer.
 * @return `true` if the write operation is successful, `false` otherwise.
 *
 * @note The function uses `perror` to provide a system-specific error message 
 *       and prints custom information to indicate where the error occurred.
 */
bool	safe_write(int fd, char *buf, size_t n)
{
	if (write(fd, buf, n) < 0)
	{
		printf("%sminishell: %d: %s%s\n", RED, fd, strerror(errno), RESET);
		return (false);
	}
	return (true);
}

/**
 * @brief Free the entire list of tokens.
 *
 * @param tokens The head of the token list.
 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
