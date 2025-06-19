/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 22:06:26 by decilapdeni      ###   ########.fr       */
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

static int	insert_match_loop(t_token *cur, t_token **inserted, char **matches)
{
	int				i;
	t_token_data	data;
	t_token			*new_tok;

	i = 0;
	while (matches[i])
	{
		data = (t_token_data){
			matches[i],
			cur->type,
			cur->quoted,
			cur->quote_char
		};
		new_tok = add_token(inserted, data);
		if (!new_tok)
		{
			free_tokens(*inserted);
			return (0);
		}
		new_tok->joined = cur->joined;
		i++;
	}
	return (1);
}


static int	insert_wildcard_matches(t_token **tokens, t_token **cur,
	char **matches)
{
	t_token	*inserted;

	inserted = NULL;
	if (!insert_match_loop(*cur, &inserted, matches))
		return (0);
	replace_token_with_list(tokens, *cur, inserted);
	*cur = inserted;
	while (*cur && (*cur)->next)
		*cur = (*cur)->next;
	return (1);
}

int	expand_current_wildcard(t_token **tokens, t_token **cur)
{
	char	**matches;

	matches = wildcard_expand((*cur)->value);
	if (!matches)
		return (1);
	if (!insert_wildcard_matches(tokens, cur, matches))
		return (0);
	ft_free_arr(matches);
	return (1);
}
