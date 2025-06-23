/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:53:04 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/20 19:22:48 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Initialise la fusion d'un bloc de tokens WORD/SUBSHELL.
 *
 * @param cur Le token de départ.
 * @param merged Pointeur vers la chaîne mergée à initialiser.
 * @param quote Pointeur vers le quote_state.
 */
static void	handle_merge_word_start(t_token *cur, char **merged,
		t_quote_state *quote, t_shell *shell)
{
	char	*curr_value;

	if (cur->value)
		curr_value = cur->value;
	else
		curr_value = "";
	*merged = ft_strdup(curr_value);
	if (!*merged)
		exit_error("ft_strdup failed in handle_merge_word_start", shell);
	*quote = cur->quote_char;
}

/**
 * @brief Continue la fusion des tokens suivants s'ils sont join.
 *
 * @param cur Le pointeur sur le token courant (modifié à l'intérieur).
 * @param merged Pointeur vers la chaîne mergée (modifié à chaque ajout).
 * @param quote Pointeur vers le quote_state (mis à jour selon les tokens).
 */
static void	handle_merge_word_continue(t_token **cur, char **merged,
	t_quote_state *quote, t_shell *shell)
{
	char	*tmp;

	while ((*cur)->joined && (*cur)->next
		&& ((*cur)->next->type == TOKEN_WORD
			|| (*cur)->next->type == TOKEN_SUBSHELL))
	{
		if (ft_strchr((*cur)->value, '$')
			&& (*cur)->quote_char == Q_DOUBLE_QUOTE)
			break ;
		*cur = (*cur)->next;
		if ((*cur)->value)
			tmp = ft_strjoin(*merged, (*cur)->value);
		else
			tmp = ft_strjoin(*merged, "");
		if (!tmp)
		{
			free(*merged);
			exit_error("ft_strjoin failed in handle_merge_word_continue", shell);
			return ;
		}
		free(*merged);
		*merged = tmp;
		if ((*cur)->quote_char > *quote)
			*quote = (*cur)->quote_char;
	}
}

/**
 * @brief Gère un bloc de token WORD avec fusions et quotes.
 */
t_token	*handle_word_token_block(t_parse_ctx *ctx, t_shell *shell)
{
	char			*merged;
	t_token			*cur;
	t_quote_state	quote;

	cur = ctx->tok;
	handle_merge_word_start(cur, &merged, &quote, shell);
	handle_merge_word_continue(&cur, &merged, &quote, shell);
	ctx->args[ctx->arg_i] = merged;
	ctx->quote_chars[ctx->quote_i] = quote;
	ctx->arg_i++;
	ctx->quote_i++;
	return (cur);
}

/**
 * @brief Checks whether a string is a valid shell variable assignment.
 *
 * Returns true if the line is a valid "name=value" assignment
 * (name starts with alpha/_ and contains only alnum/_).
 *
 * @param line  The string to check.
 * @return 1 if valid assignment, 0 otherwise.
 */
int	is_valid_assignment(const char *line)
{
	int	i;

	i = 0;
	if (!line || !ft_strchr(line, '='))
		return (0);
	if (!(ft_isalpha(line[0]) || line[0] == '_'))
		return (0);
	while (line[i] && line[i] != '=')
	{
		if (!(ft_isalnum(line[i]) || line[i] == '_'))
			return (0);
		i++;
	}
	if (line[i] != '=')
		return (0);
	return (1);
}

/**
 * @brief Full parsing flow from raw input tokens to final command structures.
 *
 * This function takes the replaced input string, performs full lexical analysis,
 * quote processing, wildcard expansion, and finally parses the tokens into
 * command structures ready for execution.
 *
 * Flow:
 *  1. Lexical analysis (lexer)
 *  2. Quote extension processing (apply_quote_extension)
 *  3. Wildcard expansion and syntax validation (validate_and_expand_wildcards)
 *  4. Final parsing into command structs (parse_tokens)
 *
 * @param replaced The input line after subshell replacement.
 * @param shell Pointer to the shell state (for env and error handling).
 * @param multi_data Pointer to heredoc data (if any).
 * @return The linked list of parsed commands or NULL if error occurred.
 */
t_cmd	*parse_tokens_flow(char *replaced, t_shell *shell, t_token *multi_data)
{
	t_token	*tokens;
	t_token	*tmp;
	t_cmd	*cmds;

	tokens = lexer(replaced, shell);
	if (!tokens)
		return (NULL);

	shell->temp_tokens = tokens;
	tmp = apply_quote_extension(tokens, shell);
	if (!tmp)
		return (NULL);
	free_tokens(tokens);
	tokens = tmp;
	shell->temp_tokens = tokens;
	tokens = validate_and_expand_wildcards(tokens, shell);
	if (!tokens)
		return (NULL);
	cmds = parse_tokens(tokens, shell, multi_data);
	free_tokens(tokens);
	shell->temp_tokens = NULL;
	return (cmds);
}

