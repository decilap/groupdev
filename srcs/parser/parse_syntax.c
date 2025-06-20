/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:35:24 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/14 22:51:44 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Checks if a command is a forbidden special case.
 *
 * Calls the specific forbidden case checkers for dot and tilde.
 *
 * @param line   Input command line to check.
 * @param shell  Pointer to the shell context for status update.
 * @return 1 if forbidden case detected, 0 otherwise.
 */
int	is_forbidden_special_case(const char *line, t_shell *shell)
{
	if (is_forbidden_dot(line, shell))
		return (1);
	if (is_forbidden_tilde(line, shell))
		return (1);
	return (0);
}

/**
 * @brief Checks if the given arithmetic expression is syntactically valid.
 *
 * Returns 1 if parentheses are balanced and there is content inside.
 * Used to validate ((...)) expressions for arithmetic evaluation.
 *
 * @param line  The arithmetic expression string.
 * @return 1 if valid, 0 otherwise.
 */
int	is_valid_arithmetic_syntax(const char *line)
{
	int	i;
	int	count;
	int	content;

	i = 0;
	count = 0;
	content = 0;
	while (line[i])
	{
		if (line[i] == '(')
			count++;
		else if (line[i] == ')')
			count--;
		else if (line[i] != ' ' && line[i] != '\t')
			content = 1;
		if (count < 0)
			return (0);
		i++;
	}
	return (count == 0 && content);
}

/**
 * @brief Validate syntax, trim spaces, and handle assignment.
 *
 * @param line The raw input command line.
 * @param shell Shell context.
 * @return Trimmed input string or NULL if error.
 */
char	*validate_and_trim(char *line, t_shell *shell)
{
	char	*trimmed;
	t_cmd	*assign;

	if (!check_line_errors(line, shell))
		return (NULL);
	trimmed = ft_strtrim(line, " \t");
	if (!trimmed)
		return (NULL);
	assign = handle_assignment(trimmed, shell);
	if (assign == NULL || assign != (t_cmd *)1)
	{
		free(trimmed);
		return (NULL);
	}
	return (trimmed);
}
