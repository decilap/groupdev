/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:28:54 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/20 20:51:14 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Removes surrounding quotes from a string if present.
 * 
 * This function checks if the input string starts and ends with
 * matching single or double quotes, and if so, removes them.
 * Memory is reallocated for the new unquoted string.
 * 
 * @param s Pointer to the string to process. Updated in place.
 */
void	remove_quotes(char **s)
{
	char	*tmp;
	size_t	len;

	tmp = *s;
	len = ft_strlen(tmp);
	if ((tmp[0] == '\'' || tmp[0] == '\"') && tmp[len - 1] == tmp[0])
	{
		tmp[len - 1] = '\0';
		*s = ft_strdup(tmp + 1);
		if (tmp[1] && !*s)
			return (perror(RED "minishell: 7 unset" RESET));
		free(tmp);
	}
}

/**
 * @brief Determines the token type based on its string value.
 * 
 * This function compares the string to known shell operators
 * and returns the corresponding token type. If none matches,
 * returns TOKEN_WORD by default.
 * 
 * @param s The string to analyze.
 * @return The corresponding token type enum.
 */
int	get_token_type(char *s)
{
	if (!s)
		return (TOKEN_WORD);
	if (ft_strcmp(s, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strcmp(s, ">") == 0)
		return (TOKEN_REDIR_OUT);
	if (ft_strcmp(s, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(s, "&&") == 0)
		return (TOKEN_AND);
	if (ft_strcmp(s, "||") == 0)
		return (TOKEN_OR);
	if (ft_strcmp(s, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(s, ";") == 0)
		return (TOKEN_POINT_VIRGULE);
	if (ft_strcmp(s, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_WORD);
}

static int	copy_token_content(t_token **curr, t_token **new_tok,
	t_token **copied)
{
	char			*copy_val;
	t_token_data	data;

	copy_val = ft_strdup((*curr)->value);
	if (!copy_val)
		return (0);
	data = (t_token_data){copy_val, (*curr)->type, (*curr)->quoted,
		(*curr)->quote_char};
	*copied = add_token(new_tok, data);
	if (!(*copied))
		return (free(copy_val), 0);
	return (1);
}

/**
 * @brief Removes the last token from a linked list of tokens.
 *
 * This function traverses the list to find the last token,
 * frees its allocated memory, and removes it from the list.
 *
 * @param head Pointer to the head of the token list.
 */
static void	remove_last_token(t_token **head)
{
	t_token	*prev;
	t_token	*curr;

	prev = NULL;
	curr = *head;
	if (!curr)
		return ;
	while (curr->next)
	{
		prev = curr;
		curr = curr->next;
	}
	if (prev)
		prev->next = NULL;
	else
		*head = NULL;
	free(curr->value);
	free(curr);
}

/**
 * @brief Copies a simple token to the new token list.
 *
 * This function duplicates the current token and adds it to the new token list.
 * If the token is an empty unquoted string and not joined to any
 * following token,
 * it removes it (empty argument suppression). The function also
 * correctly handles
 * the "joined" property for concatenated tokens.
 *
 * @param curr Pointer to the current token (gets advanced during processing).
 * @param new_tok Pointer to the head of the new token list.
 * @return 1 on success, 0 on allocation failure.
 */
int	copy_simple_token(t_token **curr, t_token **new_tok)
{
	t_token	*copied;

	if (!copy_token_content(curr, new_tok, &copied))
		return (0);
	if (copied->value && copied->value[0] == '\0' && copied->quoted == 0)
	{
		if (!copied->joined && (!(*curr)->next || !(*curr)->next->joined))
		{
			remove_last_token(new_tok);
			*curr = (*curr)->next;
			return (1);
		}
	}
	if ((*curr)->type == TOKEN_WORD || (*curr)->type == TOKEN_SUBSHELL)
		copied->joined = (*curr)->joined;
	else
		copied->joined = 0;
	*curr = (*curr)->next;
	return (1);
}
