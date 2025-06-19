/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:14:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/19 20:40:27 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Extracts the content of a subshell 
 * (parentheses block) from the input line.
 *
 * Handles nested parentheses by increasing the depth
 * for each '(' and decreasing for each ')'.
 * On success, allocates and assigns the substring
 * containing the content inside the subshell.
 *
 * @param line  The input line containing the subshell.
 * @param ctx   The parsing context (indices, depth, etc.).
 * @param sub   Pointer to store the extracted subshell content (malloc('ed).
 * @return 1 on success, 0 on unbalanced parentheses (syntax error).
 */
static int	extract_subshell_content(const char *line, t_sub_ctx *ctx,
char **sub)
{
	ctx->start = ctx->i;
	ctx->depth = 1;
	ctx->i++;
	while (line[ctx->i] && ctx->depth > 0)
	{
		if (line[ctx->i] == '(')
			ctx->depth++;
		else if (line[ctx->i] == ')')
			ctx->depth--;
		ctx->i++;
	}
	if (ctx->depth != 0)
		return (0);
	ctx->len = ctx->i - ctx->start;
	*sub = ft_strndup(line + ctx->start + 1, ctx->len - 2);
	if (!*sub)
		return (-1);
	return (1);
}

/**
 * @brief Stores a subshell's command content and replaces 
 * it with a unique placeholder tag in the result string.
 *
 * Generates a tag of the form "__SUBSHELL_N__" where N is the index,
 * stores the original subshell command in the table,
 * and appends the tag to the result.
 *
 * @param ctx The parsing context (result string, subshell table, etc.).
 * @param sub The extracted subshell command string (malloc('ed).
 * @return 1 on success, 0 if maximum subshells are reached.
 */
static int	subshell_store_and_replace(t_sub_ctx *ctx, char *sub)
{
	char	*index_str;
	char	*tag;
	char	*tmp_str;

	if (ctx->table->count >= MAX_SUBSHELLS)
		return (0);
	ctx->table->commands[ctx->table->count] = sub;
	index_str = ft_itoa(ctx->table->count);
	if (!index_str)
		return (-1);
	tag = ft_strjoin_3("__SUBSHELL_", index_str, "__");
	if (!tag)
		return (-1);
	free(index_str);
	ctx->table->count++;
	tmp_str = ft_strjoin(ctx->result, tag);
	if (!tmp_str)
		return (-1);
	free(tag);
	free(ctx->result);
	ctx->result = tmp_str;
	return (1);
}

/**
 * @brief Appends a single character to the result string in the parsing context.
 *
 * Handles memory allocation and advances the context index.
 *
 * @param ctx The parsing context (result string, temp buffer, index).
 * @param c   The character to append.
 * @return 1 on success, 0 on allocation failure.
 */
int	append_char_to_result(t_sub_ctx *ctx, char c)
{
	char	*new_result;

	ctx->tmp[0] = c;
	ctx->tmp[1] = '\0';
	new_result = ft_strjoin(ctx->result, ctx->tmp);
	if (!new_result)
		return (0);
	free(ctx->result);
	ctx->result = new_result;
	ctx->i++;
	return (1);
}

/**
 * @brief Parses and processes a subshell chunk starting
 * at the current index in the input line.
 *
 * Extracts the subshell content, stores it, and inserts a tag in the result.
 * Handles syntax errors or too many nested subshells.
 *
 * @param line The input line containing the subshell.
 * @param ctx  The parsing context.
 * @return 1 on success, 0 on error (syntax or nesting limit).
 */
int	handle_subshell_chunk(const char *line, t_sub_ctx *ctx)
{
	char	*sub;
	int		ret_ext;
	int		ret_sub;

	sub = NULL;
	ret_ext = extract_subshell_content(line, ctx, &sub);
	if (ret_ext <= 0)
	{
		print_subshell_syntax_error(line);
		free(ctx->result);
		if (ret_ext < 0)
			return (-1);
		return (0);
	}
	ret_sub = subshell_store_and_replace(ctx, sub);
	if (ret_sub <= 0)
	{
		ft_putstr_fd(RED "minishell: trop de sous-shells imbriqués\n" RESET, 2);
		free(sub);
		free(ctx->result);
		if (ret_sub < 0)
			return (-1);
		return (0);
	}
	return (1);
}

/**
 * @brief Executes a subshell command by forking and running the subshell child.
 *
 * Waits for the child process to finish and returns its exit status.
 * Handles both normal exit and signal termination.
 *
 * @param cmd   The subshell command to execute.
 * @param shell The main shell context.
 * @param ctx   The subshell execution context.
 * @return      The exit status of the subshell.
 */
int	handle_subshell(t_cmd *cmd, t_shell *shell, t_subsh_ctx *ctx)
{
	int		substat;
	int		final_status;
	pid_t	subshell_pid;

	substat = 0;
	final_status = 0;
	subshell_pid = fork();
	if (subshell_pid == -1)
	{
		perror("[handle_subshell] fork failed");
		return (1);
	}
	if (subshell_pid == 0)
		subshell_child(cmd, shell, ctx);
	waitpid(subshell_pid, &substat, 0);
	if (WIFEXITED(substat))
	{
		final_status = WEXITSTATUS(substat);
		return (final_status);
	}
	else if (WIFSIGNALED(substat))
		final_status = 128 + WTERMSIG(substat);
	shell->exit_status = final_status;
	return (final_status);
}
