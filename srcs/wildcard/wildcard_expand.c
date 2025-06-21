/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <ddecilap@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/21 12:17:08 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

int	validate_redirections(t_token *tokens, t_shell *shell)
{
	t_token	*t;

	t = tokens;
	while (t)
	{
		if ((t->type == TOKEN_REDIR_IN
				|| t->type == TOKEN_REDIR_OUT
				|| t->type == TOKEN_APPEND || t->type == TOKEN_HEREDOC)
			&& (!t->next || t->next->type != TOKEN_WORD))
		{
			shell->exit_status = 2;
			if (t->next)
				print_syntax_error(t->next->value);
			else
				print_syntax_error(NULL);
			return (0);
		}
		t = t->next;
	}
	return (1);
}

int	validate_pipe_logic(t_token *tokens, t_shell *shell)
{
	t_token	*t;
	int		error;

	t = tokens;
	if (t && (t->type == TOKEN_PIPE || t->type == TOKEN_AND
			|| t->type == TOKEN_OR))
	{
		shell->exit_status = 2;
		print_syntax_error(t->value);
		return (0);
	}
	while (t)
	{
		error = is_syntax_error(t);
		if (error)
		{
			shell->exit_status = 2;
			print_syntax_error(t->value);
			return (0);
		}
		t = t->next;
	}
	return (1);
}
