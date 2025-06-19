/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:10:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 20:40:27 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Determines which token caused a subshell syntax error.
 *
 * Checks for empty parentheses first (returns ')'), then checks for unbalanced
 * parentheses using check_parentheses_balance (returns the offending token).
 *
 * @param line The input line to check.
 * @return The unexpected token character (e.g., ')', '('), or 0 if unknown.
 */
static char	subshell_syntax_error_token(const char *line)
{
	if (has_empty_parentheses(line))
		return (')');
	return (check_parentheses_balance(line));
}

/**
 * @brief Prints a bash-style syntax error message for subshell errors.
 *
 * Prints: bash: syntax error near unexpected token `<token>`
 *
 * @param line The input line with the syntax error.
 */
void	print_subshell_syntax_error(const char *line)
{
	char	token;

	token = subshell_syntax_error_token(line);
	ft_putstr_fd("minidhell: syntax error near unexpected token\
		`", STDERR_FILENO);
	if (token)
		ft_putchar_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("(", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

/**
 * @brief Prints a bash-style syntax error message for a subshell sub-expression.
 *
 * Alias for print_subshell_syntax_error but accepts a different variable name.
 *
 * @param sub_line The subshell substring with the syntax error.
 */
void	subshell_print_syntax_error(const char *sub_line)
{
	char	token;

	token = subshell_syntax_error_token(sub_line);
	ft_putstr_fd("minidhell: syntax error near unexpected token\
		`", STDERR_FILENO);
	if (token)
		ft_putchar_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("(", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}
