/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_single_arg.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 21:13:18 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Expands variables for a single argument.
 *
 * Calls the full variable expansion logic.
 *
 * @param arg The argument string.
 * @param shell The shell context.
 * @param is_quoted 1 if quoted, 0 otherwise.
 * @param quote_char The quote type.
 * @return A token list with expanded results or NULL if error.
 */
static t_token	*expand_variables_for_arg(char *arg, t_shell *shell,
	int is_quoted, t_quote_state quote_char)
{
	t_token	*tok;

	tok = expand_variables(arg, shell, is_quoted, quote_char);
	if (!tok)
	{
		shell->exit_status = 2;
		perror(RED "minishell: execution: expand_vaiables" RESET);
		return (NULL);
	}
	return (tok);
}

/**
 * @brief Concatenates all expanded tokens into a single string.
 *
 * @param expanded The expanded token list.
 * @return The concatenated string or NULL if allocation fails.
 */
static char	*concatenate_expanded_tokens(t_token *expanded)
{
	char	*res;
	char	*tmp;
	t_token	*cur;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	cur = expanded;
	while (cur)
	{
		if (cur->value)
			tmp = ft_strjoin(res, cur->value);
		else
			tmp = ft_strjoin(res, "");
		if (!tmp)
		{
			free(res);
			return (NULL);
		}
		free(res);
		res = tmp;
		cur = cur->next;
	}
	return (res);
}

/**
 * @brief Fully expands one argument of a command.
 *
 * This performs variable expansion and replaces the argument value.
 *
 * @param cmd The command structure.
 * @param shell The shell context.
 * @param i The argument index to expand.
 */
bool	expand_single_arg(t_cmd *cmd, t_shell *shell, int i)
{
	int		is_quoted;
	char	*res;
	t_token	*expanded;

	is_quoted = (cmd->quote_chars[i] != Q_NONE);
	expanded = expand_variables_for_arg(cmd->args[i], shell, is_quoted,
			cmd->quote_chars[i]);
	if (!expanded)
		return (false);
	res = concatenate_expanded_tokens(expanded);
	free_tokens(expanded);
	if (!res)
	{
		shell->exit_status = 2;
		perror(RED "minishell: execution: concatenate_expanded_tokens" RESET);
		return (false);
	}
	free(cmd->args[i]);
	cmd->args[i] = res;
	return (true);
}
