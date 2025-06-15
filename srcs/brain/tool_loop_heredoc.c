/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_loop_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:00 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 23:00:17 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static bool	ft_init_pipe_tok(int *pipefd, const char *str)
{
	if (pipe(pipefd) == -1)
		return (false);
	if (!safe_write(pipefd[1], (char *)str, ft_strlen(str)))
		return (false);
	if (!safe_close(pipefd[1]))
		return (false);
	return (true);
}

/**
 * @brief Adds a token to the list from a single input line.
 *
 * This function trims the newline from the line, creates the token,
 * and inserts it into the linked list.
 *
 * @param tokens Pointer to token list.
 * @param line Input line to process.
 * @return 1 on success, 0 on failure.
 */
static int	add_token_from_line(t_token **tokens, char *line)
{
	char			*trim;
	t_token_data	data;

	trim = ft_strtrim(line, "\n");
	if (!trim)
		return (0);
	data = (t_token_data){trim, TOKEN_WORD, 0, Q_NONE};
	if (!add_token(tokens, data))
	{
		free(trim);
		return (0);
	}
	return (1);
}

/**
 * @brief Tokenizes a string into a list of tokens line by line.
 *
 * This function takes a string, splits it into lines using a pipe trick,
 * trims the newline character from each line, and stores each trimmed line
 * as a TOKEN_WORD into a linked list of tokens.
 * 
 * All memory allocations and errors are properly handled.
 *
 * @param str The input string to tokenize.
 * @return A linked list of tokens, or NULL if any error occurs.
 */
static t_token	*tokenize_lines_from_str(const char *str)
{
	int			pipefd[2];
	char		*line;
	t_token		*tokens;

	line = NULL;
	tokens = NULL;
	if (!ft_init_pipe_tok(pipefd, str))
		return (NULL);
	while (1)
	{
		line = get_next_line(pipefd[0]);
		if (!line)
			break ;
		if (!add_token_from_line(&tokens, line))
		{
			free(line);
			safe_close(pipefd[0]);
			free_tokens(tokens);
			return (NULL);
		}
		free(line);
	}
	safe_close(pipefd[0]);
	return (tokens);
}

static char	*extract_heredoc_delim(const char *str)
{
	const char	*ptr;
	const char	*start;

	ptr = str;
	while (*ptr)
	{
		if (*ptr == '<' && *(ptr + 1) == '<')
		{
			ptr += 2;
			while (*ptr && (*ptr == ' ' || *ptr == '\t'))
				ptr++;
			start = ptr;
			while (*ptr && !ft_isspace(*ptr))
				ptr++;
			return (ft_substr(start, 0, ptr - start));
		}
		ptr++;
	}
	return (NULL);
}

bool	ft_init_loop_heredoc(char *line, t_token **tok, char **delim,
	char **result)
{
	*tok = tokenize_lines_from_str(line);
	if (!*tok)
	{
		perror(RED "minishell: Error in tokenize_lines_from_str" RESET);
		return (false);
	}
	*delim = extract_heredoc_delim((*tok)->value);
	if (!*delim)
	{
		perror(RED "minishell: Error in extract_heredoc_delim" RESET);
		return (free_tokens(*tok), false);
	}
	*result = ft_strdup((*tok)->value);
	if (!*result)
	{
		perror(RED "minishell: (ftilh) ft_strdup" RESET);
		return (free_tokens(*tok), free(delim), false);
	}
	return (true);
}
