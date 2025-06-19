/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 18:41:51 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Extracts the error token from an arithmetic expression 
 * for error reporting.
 *
 * If 'triple' is set, returns the next word after the first space; otherwise,
 * returns the first non-space, non-tab token from the expression.
 *
 * @param expr    The arithmetic expression to analyze.
 * @param triple  Indicates special (( )) syntax context.
 * @return Dynamically allocated string containing the error 
 * token (must be freed).
 */
static char	*get_arithmetic_error_token(const char *expr, int triple)
{
	int		k;
	int		start_token;
	char	*tok;

	if (triple)
	{
		tok = ft_strchr(expr, ' ');
		if (tok)
			tok++;
		else
			tok = (char *)expr;
		return (ft_strdup(tok));
	}
	else
	{
		k = 0;
		while (expr[k] && (expr[k] == ' ' || expr[k] == '\t'))
			k++;
		start_token = k;
		while (expr[k] && expr[k] != ' ' && expr[k] != '\t')
			k++;
		return (ft_substr(expr, start_token, k - start_token));
	}
}

/**
 * @brief Prints detailed arithmetic syntax error information to stderr.
 *
 * Uses the expression and triple context to print a relevant error message,
 * including the specific error token.
 *
 * @param expr    The arithmetic expression with an error.
 * @param triple  Indicates if the error is about missing parenthesis
 * in (( )) syntax.
 */
static void	print_arithmetic_error_detail(const char *expr, int triple)
{
	char	*tok;

	tok = get_arithmetic_error_token(expr, triple);
	if (triple)
	{
		ft_putstr_fd(RED ": missing `)' (error token is \"", STDERR_FILENO);
		ft_putstr_fd(tok, STDERR_FILENO);
		ft_putstr_fd("\")\n" RESET, STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(RED ": syntax error in expression\n(error\
			 token is \"" RESET, STDERR_FILENO);
		ft_putstr_fd(tok, STDERR_FILENO);
		ft_putstr_fd("\")\n" RESET, STDERR_FILENO);
	}
	free(tok);
}

/**
 * @brief Prints an arithmetic syntax error message based on the given line.
 *
 * Detects the error context (triple or normal), extracts the failing token,
 * and prints the error in the Bash error style.
 *
 * @param line The line containing the arithmetic expression with the error.
 */
static void	print_arithmetic_error(const char *line)
{
	int		triple;
	char	*expr;

	triple = 0;
	expr = get_arithmetic_expr(line, &triple);
	ft_putstr_fd("minidhell: ((: ", STDERR_FILENO);
	if (expr)
	{
		ft_putstr_fd(expr, STDERR_FILENO);
		print_arithmetic_error_detail(expr, triple);
		free(expr);
	}
	else
	{
		ft_putstr_fd(RED ": syntax error in expression\n" RESET, STDERR_FILENO);
	}
}

/**
 * @brief Checks arithmetic syntax and special symbol syntax in the input line.
 *
 * This function first skips leading whitespace, then checks if the line starts 
 * with an arithmetic expression "((". If so, it validates the expression syntax.
 * It also checks for forbidden or incorrect usage of special characters 
 * such as '&', '|', and ';'.
 *
 * @param line  The input command line.
 * @param shell Pointer to the shell state for error reporting and exit status.
 * @return 1 if no error is detected; 0 if a syntax error is found.
 */
int	check_arithmetic_and_special_syntax(char *line, t_shell *shell)
{
	int	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == '(' && line[i + 1] == '(')
	{
		if (!is_valid_arithmetic_syntax(line))
		{
			print_arithmetic_error(line);
			shell->exit_status = 2;
			return (0);
		}
	}
	if (check_special_syntax(line, '&')
		|| check_special_syntax(line, '|')
		|| check_special_syntax(line, ';'))
	{
		shell->exit_status = 2;
		return (0);
	}
	return (1);
}

/**
 * @brief Performs global syntax checks on the input line before parsing.
 *
 * This function combines multiple pre-parsing checks: 
 * arithmetic and special character syntax validation, 
 * and forbidden special case detection.
 *
 * @param line  The input command line.
 * @param shell Pointer to the shell state for error reporting and exit status.
 * @return 1 if the line is valid; 0 if any syntax error is detected.
 */
int	check_line_errors(char *line, t_shell *shell)
{
	if (!check_arithmetic_and_special_syntax(line, shell))
		return (0);
	if (is_forbidden_special_case(line, shell))
		return (0);
	return (1);
}
