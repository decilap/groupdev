/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:02:20 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 15:27:53 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Handles the case of: $"HOME" or $'HOME'
 * 
 * When $ is followed by a quoted non-empty word or subshell.
 */
int	handle_non_empty_quote_with_dollar(t_token **curr, t_token **new_tok, t_shell *shell)
{
	char			*merged;
	t_token			*added;
	t_token_data	data;

	merged = ft_strdup((*curr)->next->value);
	if (!merged)
		return (0);
	data = (t_token_data){merged, TOKEN_WORD, (*curr)->quoted,
		(*curr)->next->quote_char};
	added = add_token(new_tok, data, shell);
	if (!added)
		return (free(merged), 0);
	added->joined = (*curr)->joined;
	*curr = (*curr)->next->next;
	return (1);
}

/**
 * @brief Simply duplicates any token containing a dollar for deferred expansion.
 * 
 * Used for cases where expansion happens later, like unquoted $VAR.
 */
int	handle_token_with_dollar(t_token **curr, t_token **new_tok, t_shell *shell)
{
	char			*copy_val;
	t_token_data	data;
	t_token			*copied;

	copy_val = ft_strdup((*curr)->value);
	if (!copy_val)
		return (0);
	data = (t_token_data){copy_val, (*curr)->type, (*curr)->quoted,
		(*curr)->quote_char};
	copied = add_token(new_tok, data, shell);
	if (!copied)
		return (free(copy_val), 0);
	copied->joined = (*curr)->joined;
	*curr = (*curr)->next;
	return (1);
}

/**
 * @brief Handles the case of: $"".HOME (empty quote followed by word/subshell).
 * 
 * When $ is followed by an empty quote and then a WORD/SUBSHELL, 
 * it merges the third token into the result.
 */
int	handle_empty_quote_with_dollar(t_token **curr, t_token **new_tok, t_shell *shell)
{
	char			*merged_val;
	t_token			*added;
	t_token_data	data;

	merged_val = ft_strdup((*curr)->next->next->value);
	if (!merged_val)
		return (0);
	data = (t_token_data){merged_val, TOKEN_WORD, (*curr)->quoted,
		(*curr)->next->quote_char};
	added = add_token(new_tok, data, shell);
	if (!added)
		return (free(merged_val), 0);
	added->joined = (*curr)->joined;
	*curr = (*curr)->next->next->next;
	return (1);
}
