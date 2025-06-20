/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:24:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/10 23:49:42 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Returns a pointer to the global (static) subshell table.
 *
 * This table stores all parsed subshell command strings
 * for the current parsing/execution.
 *
 * @return Pointer to the singleton subshell table.
 */
static t_subshell_table	*get_subshell_table(void)
{
	static t_subshell_table	table;

	return (&table);
}

/**
 * @brief Checks the balance of parentheses in a string.
 *
 * Increments for each '(', decrements for each ')'.
 * Returns ')' if a closing parenthesis appears without a matching opening one,
 * '(' if there are unclosed opening parentheses, or 0 if balanced.
 *
 * @param line The string to check.
 * @return The problematic parenthesis character (')' or '('), or 0 if balanced.
 */
char	check_parentheses_balance(const char *line)
{
	int	count;

	count = 0;
	while (*line)
	{
		if (*line == '(')
			count++;
		else if (*line == ')')
		{
			count--;
			if (count < 0)
				return (')');
		}
		line++;
	}
	if (count > 0)
		return ('(');
	return (0);
}

/**
 * @brief Replaces each subshell in the input line with a unique tag,
 * storing the commands.
 *
 * Parses the line, detects subshell expressions, and replaces them with tags
 * (e.g., "__SUBSHELL_N__"). The original commands are stored
 * in the global table.
 *
 * @param line The input string to process.
 * @return A new string with subshells replaced by tags (must be freed),
 * or NULL on error.
 */
char	*replace_subshells(const char *line)
{
	t_sub_ctx	ctx;

	ctx.i = 0;
	ctx.result = ft_strdup("");
	if (!ctx.result)
		return (NULL);
	ctx.table = get_subshell_table();
	while (line[ctx.i])
	{
		if (line[ctx.i] == '(')
		{
			if (!handle_subshell_chunk(line, &ctx))
				return (NULL);
		}
		else
		{
			if (!append_char_to_result(&ctx, line[ctx.i]))
			{
				free(ctx.result);
				return (NULL);
			}
		}
	}
	return (ctx.result);
}

/**
 * @brief Retrieves the original subshell command for a given tag.
 *
 * Looks up the tag "__SUBSHELL_N__" in the global subshell table and returns
 * the associated command string.
 *
 * @param tag The subshell tag (e.g., "__SUBSHELL_2__").
 * @return Pointer to the original command string, or NULL if not found.
 */
char	*get_subshell_command(const char *tag)
{
	int					id;
	t_subshell_table	*table;

	table = get_subshell_table();
	if (ft_strncmp(tag, "__SUBSHELL_", 11) != 0)
		return (NULL);
	id = ft_atoi(tag + 11);
	if (id < 0 || id >= table->count)
		return (NULL);
	return (table->commands[id]);
}

/**
 * @brief Frees all subshell commands and resets the global subshell table.
 *
 * Should be called after parsing/executing to avoid memory leaks.
 */
void	clear_subshell_table(void)
{
	int					i;
	t_subshell_table	*table;

	i = 0;
	table = get_subshell_table();
	while (i < table->count)
	{
		free(table->commands[i]);
		table->commands[i] = NULL;
		i++;
	}
	table->count = 0;
}
