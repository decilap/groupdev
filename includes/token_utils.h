/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:48 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/20 20:50:23 by ryoussfi         ###   ########.fr       */
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
void			print_syntax_error(char *token);
int				get_token_type(char *s);
int				is_quoted(const char *delim_raw);
int				is_syntax_error(t_token *t);
int				copy_simple_token(t_token **curr, t_token **new_tok);
t_token			*add_token(t_token **head, t_token_data data);
t_quote_state	merge_quote_state(t_quote_state a, t_quote_state b);

#endif
