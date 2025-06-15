/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:02:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/05/25 12:32:15 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*resize_buffer(char *buf, size_t *current_size)
{
	char	*new_buf;
	size_t	i;
	size_t	new_size;

	i = 0;
	new_size = (*current_size) * 2;
	new_buf = malloc(new_size);
	if (!new_buf)
		return (NULL);
	while (i < *current_size)
	{
		new_buf[i] = buf[i];
		i++;
	}
	free(buf);
	*current_size = new_size;
	return (new_buf);
}

static int	read_line(int fd, char **buffer, size_t *pos, size_t *size)
{
	char	c;
	ssize_t	bytes_read;

	while (1)
	{
		bytes_read = read(fd, &c, 1);
		if (bytes_read <= 0)
			break ;
		if (c == '\r')
			continue ;
		if (*pos + 1 >= *size)
		{
			*buffer = resize_buffer(*buffer, size);
			if (!*buffer)
				return (-1);
		}
		(*buffer)[(*pos)++] = c;
		if (c == '\n')
			break ;
	}
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	char	*buf;
	int		status;
	size_t	pos;
	size_t	size;

	pos = 0;
	size = BUFFER_SIZE;
	if (fd < 0)
		return (NULL);
	buf = malloc(size);
	if (!buf)
		return (NULL);
	status = read_line(fd, &buf, &pos, &size);
	if (status <= 0 && pos == 0)
	{
		free(buf);
		return (NULL);
	}
	buf[pos] = '\0';
	return (buf);
}
