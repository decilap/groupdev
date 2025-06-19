/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:34 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:34:22 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/**
 * @brief Parsing context structure used during token-to-command conversion.
 *
 * Maintains the current state of parsing: argument list, command list,
 * token position, and shell context.
 */
typedef struct s_parse_ctx
{
	int				arg_i;
	int				quote_i;
	char			**args;
	t_cmd			*head;
	t_cmd			*curr;
	t_token			*tok;
	t_shell			*shell;
	t_quote_state	*quote_chars;
}	t_parse_ctx;

t_group	*group_commands(t_cmd *cmds);
t_group	*create_group(t_cmd *cmds);
t_cmd	*parse_input(char *line, t_shell *shell, t_token *tok);
t_cmd	*setup_new_cmd(void);
t_cmd	*handle_assignment(char *trimmed, t_shell *shell);
t_cmd	*parse_tokens(t_token *tok, t_shell *shell, t_token *multi_data);
t_cmd	*parse_tokens_flow(char *replaced, t_shell *shell, t_token *multi_data);
t_cmd	*group_add_subshell(t_cmd *cmds, t_group **head,
			t_group **curr_group);
char	*validate_and_trim(char *line, t_shell *shell);
char	*get_arithmetic_expr(const char *line, int *triple);
char	*expand_token_value(const char *input, t_shell *shell, int quoted,
			t_quote_state quote_char);
char	*extract_first_word(const char *line);
t_token	*handle_word_token_block(t_parse_ctx *ctx);
t_token	*expand_and_resolve_path(t_cmd *cmd, t_shell *shell, int *err_code);
t_token	*apply_quote_extension(t_token *tokens, t_shell *shell);
void	free_tmp_args(char **args, int count);
void	handle_redirection_token(t_token **tok, t_cmd *curr, t_shell *shell);
void	free_partial_args(char **args, int count);
void	handle_path_errors(t_cmd *cmd, t_token *expanded, int err_code,
			t_shell *shell);
void	finalize_cmd_args(t_parse_ctx *ctx);
void	resolve_cmd_path(t_cmd *cmd, t_shell *shell);
void	cleanup_tmp_args(t_parse_ctx *ctx);
int		check_line_errors(char *line, t_shell *shell);
int		handle_heredoc_token(t_token **tok, t_shell *shell, t_cmd *curr,
			t_token *tok_multi_data);
int		check_special_syntax(const char *line, char type);
int		is_forbidden_special_case(const char *line, t_shell *shell);
int		process_token_subshell(t_parse_ctx *ctx);
int		is_unclosed_quote_error(const char *line, t_shell *shell);
int		is_valid_arithmetic_syntax(const char *line);
int		has_unclosed_quote(const char *line);
int		is_forbidden_special_case(const char *line, t_shell *shell);
int		is_forbidden_tilde(const char *line, t_shell *shell);	
int		is_forbidden_dot(const char *line, t_shell *shell);
int		is_valid_assignment(const char *line);
#endif
