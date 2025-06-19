/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_assignment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/17 13:42:41 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Processes an assignment to a shell local variable.
 *
 * Sets the local variable if the name is valid and not "_".
 * Updates the shell exit status. Frees input memory.
 *
 * @param name   Name of the variable (must be malloc('ed).
 * @param value  Value to assign (must be malloc('ed).
 * @param shell  Shell context for variable assignment.
 * @return Always NULL.
 */
static t_cmd	*process_assignment(char *name, char *value, t_shell *shell)
{
	if (ft_strcmp(name, "_") == 0)
	{
		ft_putstr_fd(RED "minishell: _: readonly variable\
			\n" RESET, STDERR_FILENO);
		free(name);
		free(value);
		shell->exit_status = 1;
		return (NULL);
	}
	set_local_var(shell, name, value);
	free(name);
	free(value);
	shell->exit_status = 0;
	return (NULL);
}

/**
 * @brief Handles a shell variable assignment in the input.
 *
 * If the input is a valid assignment, processes it and sets the variable.
 * Otherwise, returns a sentinel value for "not an assignment".
 *
 * @param trimmed  Input string (trimmed) to check for assignment.
 * @param shell    Shell context.
 * @return NULL if assignment handled, (t_cmd*)1 if not an assignment.
 */
t_cmd	*handle_assignment(char *trimmed, t_shell *shell)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	int		name_len;

	if (ft_strchr(trimmed, '=') && is_valid_assignment(trimmed))
	{
		equal_sign = ft_strchr(trimmed, '=');
		name_len = equal_sign - trimmed;
		name = ft_substr(trimmed, 0, name_len);
		value = ft_strdup(equal_sign + 1);
		return (process_assignment(name, value, shell));
	}
	return ((t_cmd *)1);
}

/**
 * @brief Extracts the arithmetic expression from the input line.
 *
 * Locates the arithmetic expression between parentheses ((...)) and returns it.
 * Also sets a flag if the expression uses triple-parentheses (( )).
 *
 * @param line   The input line containing the arithmetic expression.
 * @param triple Pointer to int set to 1 if (( )) arithmetic; otherwise 0.
 * @return Dynamically allocated string with the expression, or NULL.
 */
char	*get_arithmetic_expr(const char *line, int *triple)
{
	const char	*start;
	const char	*end;

	start = ft_strchr(line, '(');
	if (start && *(start + 1) == '(')
		start += 2;
	else
		start = line;
	end = ft_strrchr(line, ')');
	if (!end)
		end = line + ft_strlen(line);
	*triple = (start > line && *(start) == '(');
	if (*triple && end && *(end - 1) == ')')
		end--;
	if (start < end)
		return (ft_strndup(start, end - start));
	else
		return (NULL);
}
