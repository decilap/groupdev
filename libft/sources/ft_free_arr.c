/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_arr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:43:40 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/15 11:43:26 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_arr(char **str)
{
	size_t	i;

	if (!str)
		return;
	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}
