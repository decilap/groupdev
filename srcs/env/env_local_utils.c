/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_local_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:12:32 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 23:27:51 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Recherche une variable locale par son nom dans la liste
 * des variables locales.
 * 
 * @param shell Le shell contenant la liste des variables locales.
 * @param name Le nom de la variable à rechercher.
 * @return Un pointeur vers la variable trouvée, ou NULL si non trouvée.
 */
t_var	*find_local_var(t_shell *shell, const char *name)
{
	t_var	*var;

	var = shell->locals;
	while (var)
	{
		if (ft_strcmp(var->key, name) == 0)
			return (var);
		var = var->next;
	}
	return (NULL);
}
