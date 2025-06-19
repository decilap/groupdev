/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ry_safe_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:15:09 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/19 20:19:06 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
		ft_putstr_fd(RED "minishell: failed to write fd=", STDIN_FILENO);
		ft_putnbr_fd(fd, STDIN_FILENO);
		ft_putstr_fd(": " RESET, STDIN_FILENO);
		perror(NULL);
		return (false);
	}
	return (true);
}
