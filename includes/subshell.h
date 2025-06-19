/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:41:34 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SUBSHELL_H
# define SUBSHELL_H

/**
 * @brief Table for storing all parsed subshell command strings.
 *
 * Holds up to MAX_SUBSHELLS commands and tracks the current count.
 * Used for mapping placeholder tags to actual subshell command strings.
 */
typedef struct s_subshell_table
{
	int		count;
	char	*commands[MAX_SUBSHELLS];
}	t_subshell_table;

/**
 * @brief Parsing context for extracting and replacing
 * subshells in an input line.
 *
 * Tracks current position, depth, buffer for results,
 * and reference to the subshell table.
 */
typedef struct s_sub_ctx
{
	int					i;
	int					len;
	int					start;
	int					depth;
	char				*result;
	char				tmp[2];
	t_subshell_table	*table;
}	t_sub_ctx;

char	*replace_subshells(const char *line);
char	*get_subshell_command(const char *tag);
char	check_parentheses_balance(const char *line);
void	clear_subshell_table(void);
void	subshell_print_syntax_error(const char *sub_line);
void	subshell_clean_exit(t_shell *shell, char **env_backup, int exit_code);
int		is_subshell(t_cmd *cmd);
int		step_subshell(t_cmd *cmd, t_shell *shell, t_pipeline_ctx *ctx);
int		handle_subshell_chunk(const char *line, t_sub_ctx *ctx);
int		append_char_to_result(t_sub_ctx *ctx, char c);
int		has_empty_parentheses(const char *line);
int		is_blank(const char *str);
int		check_parentheses(const char *str);
int		s_subshell(t_cmd *cmd);

#endif