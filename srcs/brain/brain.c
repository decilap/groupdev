/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:05 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 11:33:14 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

static t_token	*ft_loop_with_herdoc(char **line, int *i, char *delim)
{
	t_token	*tok;

	tok = NULL;
	
	if (!ft_init_loop_herdoc(line, &tok, i, delim))
	{
		free(delim);
		return (perror(RED "minishell: Error in ft_init_loop_herdoc" RESET),
			NULL);
	}
	free(delim);
	return (tok);
}

static bool	ft_herdoc_chr(char *line, char **eof_delim)
{
	char	**first_line_words;
	int		i;

	first_line_words = ft_split(line, ' ');
	if (!first_line_words)
		return (false);
	i = 0;
	while (first_line_words && first_line_words[i])
	{
		if (ft_strcmp(first_line_words[i], "<<") == 0
			&& first_line_words[i + 1] && !first_line_words[i + 2])
		{
			*eof_delim = ft_strdup(first_line_words[i + 1]);
			if (!*eof_delim)
				return (ft_free_arr(first_line_words), false);
			break ;
		}
		i++;
	}
	ft_free_arr(first_line_words);
	return (true);
}

static int	has_complete_multiline_heredoc(char **lines, char **eof_delim)
{
	int		j;

	if (!lines[1])
		return (0);
	if (!ft_herdoc_chr(lines[0], eof_delim))
		return (perror(RED "minishell: ft_herdoc_chr" RESET), -1);
	if (!*eof_delim)
		return (0);
	j = 1;
	while (lines[j])
	{
		if (ft_strcmp(lines[j], *eof_delim) == 0)
			return (1);
		j++;
	}
	return (2);
}

bool	ft_loop(t_shell *shell, char **line, int *i, t_token **tok)
{
	int		ret_multi;
	char	*eof_delim;

	eof_delim = NULL;
	ret_multi = has_complete_multiline_heredoc(line, &eof_delim);
	if (ret_multi < 0)
	{
		shell->exit_status = 1;
		perror(RED "minishell: Error in has_complete_multiline_heredoc" RESET);
		return (false);
	}
	if (ret_multi > 0)
	{
		*tok = ft_loop_with_herdoc(line, i, eof_delim);
		if (*tok)
			return (true);
		shell->exit_status = 1;
		perror(RED "minishell: Error in ft_loop_with_herdoc" RESET);
		return (false);
	}
	return (true);
}
