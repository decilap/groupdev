/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:02:20 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 12:05:06 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Handles the case of $"HOME" inside double quotes.
 * 
 * Merges the "$" token with the next WORD token, producing a single token 
 * wrapped as single-quoted to avoid further expansions.
 */
int	handle_dollar_in_double_quote(t_token **curr, t_token **new_tok)
{
	t_token			*added;
	t_token_data	data;
	char			*merged;

	merged = ft_strjoin((*curr)->value, (*curr)->next->value);
	if (!merged)
		return (0);
	data = (t_token_data){
		.value = merged,
		.type = TOKEN_WORD,
		.quoted = 1,
		.quote_char = Q_SINGLE_QUOTE
	};
	added = add_token(new_tok, data);
	free(merged);

	if (!added)
		return (0);
	added->joined = 0;
	*curr = (*curr)->next->next;
	return (1);
}


/**
 * @brief Handles expansion of a token containing '$' inside double quotes.
 * 
 * Performs lazy expansion. If expansion results in empty string, skips token.
 * 
 * Return values:
 * - 0: error
 * - 2: expansion handled (skip current token)
 */
int	handle_expansion_with_dollar(t_token **curr, t_shell *shell)
{
	char	*expanded;

	expanded = expand_token_value((*curr)->value, shell, (*curr)->quoted,
			(*curr)->quote_char);
	if (!expanded)
		return (0);
	if (expanded[0] == '\0')
	{
		free(expanded);
		*curr = (*curr)->next;
		return (2);
	}
	free(expanded);
	return (2);
}

/**
 * @brief Handles the case of: $"HOME" or $'HOME'
 * 
 * When $ is followed by a quoted non-empty word or subshell.
 */
int	handle_non_empty_quote_with_dollar(t_token **curr, t_token **new_tok)
{
	t_token			*added;
	t_token_data	data;

	data = (t_token_data){
		(*curr)->next->value,
		TOKEN_WORD,
		(*curr)->quoted,
		(*curr)->next->quote_char
	};
	added = add_token(new_tok, data);
	if (!added)
		return (0);
	added->joined = (*curr)->joined;
	*curr = (*curr)->next->next;
	return (1);
}


/**
 * @brief Simply duplicates any token containing a dollar for deferred expansion.
 * 
 * Used for cases where expansion happens later, like unquoted $VAR.
 */
int	handle_token_with_dollar(t_token **curr, t_token **new_tok)
{
	t_token_data	data;
	t_token			*copied;

	data = (t_token_data){
		(*curr)->value,
		(*curr)->type,
		(*curr)->quoted,
		(*curr)->quote_char
	};
	copied = add_token(new_tok, data);
	if (!copied)
		return (0);
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
int	handle_empty_quote_with_dollar(t_token **curr, t_token **new_tok)
{
	t_token			*added;
	t_token_data	data;

	data = (t_token_data){
		(*curr)->next->next->value,
		TOKEN_WORD,
		(*curr)->quoted,
		(*curr)->next->quote_char
	};
	added = add_token(new_tok, data);
	if (!added)
		return (0);
	added->joined = (*curr)->joined;
	*curr = (*curr)->next->next->next;
	return (1);
}


