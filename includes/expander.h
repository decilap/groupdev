/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:45:27 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

/**
 * @brief Context for variable/word expansion in the shell.
 *
 * Holds all necessary pointers and state for traversing and
 * expanding an input string,
 * including current index, quote state, and access to the shell environment.
 */
typedef struct s_expander
{
	const char		*input;
	char			**result;
	size_t			*i;
	t_shell			*shell;
	t_quote_state	*state;
}	t_expander;

/**
 * @brief Initializer structure for setting up an expander context.
 *
 * Provides initial pointers and state required to initialize a t_expander.
 * Useful for functions that want to encapsulate all expander setup data.
 */
typedef struct s_expander_init
{
	const char		*input;
	char			**result;
	size_t			*i;
	t_shell			*shell;
	t_quote_state	*state;
}	t_expander_init;

t_token	*expand_and_validate_tokens(t_token *tokens, t_shell *shell);
t_token	*expand_variables(const char *input, t_shell *shell, int quoted,
			t_quote_state quote_char);
t_token	*expand_variables_heredoc(const char *input, t_shell *shell, int quoted,
			t_quote_state quote_char);
void	expand_tokens(t_token *tokens, t_shell *shell);
char	*get_variable_value(t_expander *ctx, size_t j, size_t *next_pos);
char	*expand_init_result(const char *input, size_t *i,
			t_shell *shell);
char	*expand_home(const char *input, size_t *i, t_shell *shell,
			char *result);
char	*str_append(char *base, const char *to_append);
char	*expand_tilde_filename(char *filename, t_shell *shell);
int		expand_var(t_expander *ctx, size_t j);
int		expand_none(t_expander *ctx);
int		expand_single(t_expander *ctx);
int		expand_double(t_expander *ctx);
int		expand_heredoc(t_expander *ctx);
int		expand_set_state(t_quote_state *state, size_t *i,
			t_quote_state new_state);
int		expand_append_char(char **result, char c, size_t *i);
int		expand_dollar(t_expander *ctx);
#endif
