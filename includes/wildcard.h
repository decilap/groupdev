/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:57:49 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 02:04:47 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

/**
 * @brief Context for collecting results during wildcard (glob) expansion.
 *
 * Holds a dynamically allocated array of matching filenames, the number of
 * matches,
 * and the current allocated capacity for the results array.
 */
typedef struct s_wildcard_ctx
{
	char	**results;
	int		count;
	int		capacity;
}	t_wildcard_ctx;

t_token	*validate_and_expand_wildcards(t_token *tokens, t_shell *shell);
t_token *expand_wildcard_for_token(t_token **tokens, t_token *cur);
char	**wildcard_expand(const char *pattern);
int		match_pattern(const char *pattern, const char *str);
int		validate_pipe_logic(t_token *tokens, t_shell *shell);
int		validate_redirections(t_token *tokens, t_shell *shell);
int		expand_current_wildcard(t_token **tokens, t_token **cur);
void	sort_ascii(char **array, int n);
void	free_wildcard_ctx(t_wildcard_ctx *ctx);
void	replace_token_with_list(t_token **head, t_token *target,
			t_token *new_tokens);

#endif
