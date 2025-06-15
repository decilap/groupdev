/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:55 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:47:43 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef struct s_lexer_ctx
{
	const char	*line;
	int			i;
	int			start;
	int			end;
	int			last_token_end;
	t_token		*tokens;
	t_token		*prev;
	t_token		*curr;
}		t_lexer_ctx;

typedef struct s_parser_ctx
{
	char			**args;
	int				*arg_i;
	int				*quote_i;
	t_quote_state	*quote_chars;
}	t_parser_ctx;

typedef struct s_word_ctx
{
	int				quoted;
	char			*trimmed;
	t_quote_state	quote_char;
}	t_word_ctx;

int		is_quoted_word(const char *str);
int		is_token_joined(const char *line, int curr_end, int next_start);
int		is_unquoted_word_char(const char *line, int i);
int		handle_dollar_quote(t_lexer_ctx *ctx);
int		add_trimmed_dollar_token(t_lexer_ctx *ctx, char *quoted_word);
void	lexer_handle_redir(t_lexer_ctx *ctx);
void	lexer_handle_word(t_lexer_ctx *ctx);
char	*extract_word(const char *line, int *i, int *quoted_out,
			t_quote_state *quote_char_out);
t_token	*lexer(const char *line);

#endif
