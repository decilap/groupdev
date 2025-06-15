/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:48 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:54:43 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_UTILS_H
# define TOKEN_UTILS_H

typedef struct s_token_data
{
	char			*value;
	int				type;
	int				quoted;
	t_quote_state	quote_char;
}	t_token_data;

const char		*token_type_to_str(int type);
void			remove_quotes(char **s);
void			remove_last_token(t_token **head);
void			print_syntax_error(char *token);
int				get_token_type(char *s);
int				is_quoted(const char *delim_raw);
int				is_syntax_error(t_token *t);
int				copy_simple_token(t_token **curr, t_token **new_tok);
t_token			*add_token(t_token **head, t_token_data data);
t_quote_state	merge_quote_state(t_quote_state a, t_quote_state b);

#endif
