/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_extension.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:02:20 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/23 16:02:40 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Handles the case where we have a "$" followed by a word
 * inside double quotes.
 * 
 * This merges both tokens into a single one, used for cases
 * like: "$HOME" inside quotes.
 * 
 * @return 1 if handled, 0 if not applicable or error.
 */
static int	handle_merge_dollar_with_word(t_token **curr, t_token **new_tok, t_shell *shell)
{
	char			*merged;
	t_token			*added;
	t_token_data	data;

	merged = NULL;
	added = NULL;
	if ((*curr)->value && ft_strequ((*curr)->value, "$")
		&& (*curr)->quoted && (*curr)->quote_char == Q_DOUBLE_QUOTE
		&& (*curr)->joined && (*curr)->next
		&& (*curr)->next->type == TOKEN_WORD)
	{
		merged = ft_strjoin((*curr)->value, (*curr)->next->value);
		if (!merged)
			return (0);
		data = (t_token_data){merged, TOKEN_WORD, 1, Q_SINGLE_QUOTE};
		added = add_token(new_tok, data, shell);
		if (!added)
			return (free(merged), 0);
		added->joined = 0;
		*curr = (*curr)->next->next;
		return (1);
	}
	return (0);
}

/**
 * @brief Handles lazy expansion inside double quotes ("$VAR").
 * 
 * If the expansion returns an empty string, we consume the token.
 * Otherwise, expansion is postponed.
 * 
 * @return 2 if consumed, -1 if error, 0 if not applicable.
 */
static int	handle_lazy_expand_double_quote(t_token **curr, t_shell *shell)
{
	char	*expanded;

	if ((*curr)->value && ft_strchr((*curr)->value, '$')
		&& (*curr)->quoted && (*curr)->quote_char == Q_DOUBLE_QUOTE
		&& (*curr)->joined && (*curr)->next)
	{
		expanded = expand_token_value((*curr)->value, shell, (*curr)->quoted,
				(*curr)->quote_char);
		if (!expanded)
			return (-1);
		if (expanded[0] == '\0')
		{
			free(expanded);
			*curr = (*curr)->next;
			return (2);
		}
		free(expanded);
		return (2);
	}
	return (0);
}

/**
 * @brief Handles case like: $"".HOME (empty quote followed by word).
 * 
 * We merge the word after the empty quote into the current token.
 * 
 * @return 1 if handled, 0 if not applicable or error.
 */
static int	handle_empty_quote_followed_by_word(t_token **curr,
	t_token **new_tok, t_shell *shell)
{
	char			*merged_val;
	t_token			*added;
	t_token_data	data;

	if ((*curr)->value && ft_strequ((*curr)->value, "$") && (*curr)->joined
		&& !(*curr)->quoted && (*curr)->next && (*curr)->next->quoted
		&& ft_strlen((*curr)->next->value) == 0 && (*curr)->next->joined
		&& (*curr)->next->next
		&& ((*curr)->next->next->type == TOKEN_WORD
			|| (*curr)->next->next->type == TOKEN_SUBSHELL))
	{
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
	return (0);
}

/**
 * @brief Handles case: $ followed by quoted word or subshell.
 * 
 * This covers cases like: $'HOME' or $"HOME"
 * 
 * @return 1 if handled, 0 if not applicable or error.
 */
static int	handle_dollar_followed_by_quoted_word(t_token **curr,
	t_token **new_tok, t_shell *shell)
{
	char			*merged;
	t_token			*added;
	t_token_data	data;

	if ((*curr)->value && ft_strequ((*curr)->value, "$") && (*curr)->joined
		&& !(*curr)->quoted && (*curr)->next && (*curr)->next->quoted
		&& ft_strlen((*curr)->next->value) > 0
		&& ((*curr)->next->type == TOKEN_WORD
			|| (*curr)->next->type == TOKEN_SUBSHELL))
	{
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
	return (0);
}

/**
 * @brief The main quote extension logic. Applies all quote merging rules.
 *
 * This function handles all complicated quote cases like: $"", $'var',
 * lazy $ expansion, etc.
 *
 * @return The new token list after processing, or NULL on failure.
 */
t_token	*apply_quote_extension(t_token *tokens, t_shell *shell)
{
	t_token	*curr;
	t_token	*new_tok;
	int		res;

	curr = tokens;
	new_tok = NULL;
	while (curr)
	{
		if (handle_merge_dollar_with_word(&curr, &new_tok, shell))
			continue ;
		res = handle_lazy_expand_double_quote(&curr, shell);
		if (res == -1)
			return (free_tokens(new_tok), NULL);
		if (res == 2)
			continue ;
		if (handle_empty_quote_followed_by_word(&curr, &new_tok, shell))
			continue ;
		if (handle_dollar_followed_by_quoted_word(&curr, &new_tok, shell))
			continue ;
		if (handle_token_with_dollar(&curr, &new_tok, shell))
			continue ;
		if (!copy_simple_token(&curr, &new_tok, shell))
			return (free_tokens(new_tok), NULL);
	}
	return (new_tok);
}
