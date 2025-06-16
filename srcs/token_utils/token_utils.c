/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: san <san@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:52:59 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/16 12:46:42 by san              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Adds a new token to the end of a linked list of tokens.
 *  Exits the program if allocation fails.
 *
 * This function creates a new token node, initializes it with the provided
 * value and type, and appends it to the end of the linked list. If the list
 * is empty, the new token becomes the head of the list.
 *
 * @param head A double pointer to the head of the token list.
 * @param value The string value of the token.
 * @param type An integer representing the type of the token.
 * @return A pointer to the newly added token.
 */
t_token	*add_token(t_token **head, t_token_data data)
{
	t_token	*new;
	t_token	*tmp;

	new = safe_malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = data.type;
	new->next = NULL;
	new->quoted = data.quoted;
	new->quote_char = data.quote_char;
	new->value = data.value;
	new->joined = 0;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (new);
}

/**
 * @brief Processes the input string character by character for expansion.
 *
 * This function handles dollar expansions and normal character copying
 * by iterating through the input string.
 *
 * @param ctx Pointer to the expansion context.
 * @return 1 on success, 0 on failure.
 */
static int	expand_process_loop(t_expander *ctx)
{
	while (ctx->input[*(ctx->i)])
	{
		if (ctx->input[*(ctx->i)] == '$')
		{
			if (!expand_dollar(ctx))
				return (0);
		}
		else
		{
			if (!expand_append_char(ctx->result, ctx->input[*(ctx->i)], ctx->i))
				return (0);
		}
	}
	return (1);
}

/**
 * @brief Expands the input token value according to shell expansion rules.
 *
 * If the token is single-quoted, no expansion is done.
 * Otherwise, performs dollar expansions and character appending.
 *
 * @param input The input token string to expand.
 * @param shell Pointer to the shell context for environment access.
 * @param quoted 1 if the token is quoted, 0 otherwise.
 * @param quote_char The quote type (single or double quote).
 * @return A newly allocated expanded string, or NULL on failure.
 */
char	*expand_token_value(const char *input, t_shell *shell, int quoted,
	t_quote_state quote_char)
{
	char		*result;
	size_t		i;
	t_expander	ctx;

	if (quoted && quote_char == Q_SINGLE_QUOTE)
		return (ft_strdup(input));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	ctx.input = input;
	ctx.shell = shell;
	ctx.result = &result;
	i = 0;
	ctx.i = &i;
	if (!expand_process_loop(&ctx))
	{
		free(result);
		return (NULL);
	}
	return (result);
}

/**
 * @brief Finds the previous token before the target in the list.
 *
 * @param head The head of the token list.
 * @param target The target token to find.
 * @return Pointer to the previous token, or NULL if target is head.
 */
static t_token	*find_previous_token(t_token *head, t_token *target)
{
	t_token	*prev;
	t_token	*cur;

	prev = NULL;
	cur = head;
	while (cur && cur != target)
	{
		prev = cur;
		cur = cur->next;
	}
	return (prev);
}

/**
 * @brief Replaces a token in the list with a new list of tokens.
 *
 * If new_tokens is NULL, it simply removes the target token.
 * Otherwise, it inserts new_tokens in place of target.
 *
 * @param head Pointer to the head of the token list.
 * @param target The token to replace.
 * @param new_tokens The new token list to insert (can be NULL).
 */
void	replace_token_with_list(t_token **head, t_token *target,
	t_token *new_tokens)
{
	t_token	*prev;
	t_token	*last;

	prev = find_previous_token(*head, target);
	if (!target)
		return ;
	if (!new_tokens)
	{
		if (prev)
			prev->next = target->next;
		else
			*head = target->next;
		free_tokens(target);
		return ;
	}
	if (prev)
		prev->next = new_tokens;
	else
		*head = new_tokens;
	last = new_tokens;
	while (last->next)
		last = last->next;
	last->next = target->next;
	free_tokens(target);
}
