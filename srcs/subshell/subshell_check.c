/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 11:44:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/01 15:30:06 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Checks if a string is blank (contains only whitespace).
 *
 * @param str The string to check.
 * @return 1 if the string is empty or contains only spaces,
 * tabs, or newlines; 0 otherwise.
 */
int	is_blank(const char *str)
{
	while (str && (*str == ' ' || *str == '\t' || *str == '\n'))
		str++;
	return (!str || !*str);
}

/**
 * @brief Checks for empty parentheses at the start of a line: "(    )".
 *
 * Skips leading spaces and tabs, then looks for a pair of parentheses
 * possibly separated by whitespace.
 *
 * @param line The input line.
 * @return 1 if empty parentheses are found, 0 otherwise.
 */
int	has_empty_parentheses(const char *line)
{
	int	i;
	int	j;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '(')
	{
		j = i + 1;
		while (line[j] && (line[j] == ' ' || line[j] == '\t'))
			j++;
		if (line[j] == ')')
			return (1);
	}
	return (0);
}

/**
 * @brief Checks if the string starts with only whitespace and
 * then a closing parenthesis.
 *
 * @param p The string to check.
 * @return 1 if the first non-space character is ')', 0 otherwise.
 */
static int	is_empty_parentheses(const char *p)
{
	while (*p && (*p == ' ' || *p == '\t'))
		p++;
	return (*p == ')');
}

/**
 * @brief Handles opening parenthesis when checking for balanced parentheses.
 *
 * Increments the count of open parentheses and returns 0
 * if immediately followed by an empty parenthesis.
 *
 * @param str The current position in the input string (should be at '(').
 * @param count Pointer to the parentheses nesting level.
 * @return 1 if valid; 0 if immediately followed by an empty parenthesis.
 */
static int	handle_open_parenth(const char *str, int *count)
{
	(*count)++;
	if (is_empty_parentheses(str + 1))
		return (0);
	return (1);
}

/**
 * @brief Checks if parentheses in a string are balanced and contain content.
 *
 * Verifies that every '(' is matched by a ')', no parenthesis is empty, and
 * there is at least one non-parenthesis character inside.
 *
 * @param str The input string to check.
 * @return 1 if parentheses are balanced and not empty, 0 otherwise.
 */
int	check_parentheses(const char *str)
{
	int	count;
	int	has_content;

	count = 0;
	has_content = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '(')
		{
			if (!handle_open_parenth(str, &count))
				return (0);
		}
		else if (*str == ')')
		{
			count--;
			if (count < 0)
				return (0);
		}
		else
			has_content = 1;
		str++;
	}
	return (count == 0 && has_content);
}
