/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/16 22:22:38 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Appends one string to another, returning a new allocated string.
 *
 * Concatenates 'to_append' to 'base'. Frees the original 'base'.
 *
 * @param base      The base string (will be freed).
 * @param to_append The string to append.
 * @return Newly allocated concatenated string, or NULL on allocation failure.
 */
/**
 * @brief Appends a string to an existing string, reallocating as needed.
 *
 * If base is NULL, it simply duplicates to_append.
 */
char	*str_append(char *base, const char *to_append)
{
	size_t	len_base;
	size_t	len_append;
	char	*new_str;

	if (base)
		len_base = ft_strlen(base);
	else
		len_base = 0;
	len_append = ft_strlen(to_append);
	new_str = my_malloc(len_base + len_append + 1);
	if (!new_str)
		return (NULL);
	if (base)
		ft_memcpy(new_str, base, len_base);
	ft_memcpy(new_str + len_base, to_append, len_append);
	new_str[len_base + len_append] = '\0';
	if (base)
		free(base);
	return (new_str);
}

/**
 * @brief Sets the quote state and advances the index.
 *
 * Used during word expansion to update the current quote state
 * and move to the next character.
 *
 * @param state     Pointer to the current quote state.
 * @param i         Pointer to the current index (will be incremented).
 * @param new_state The new state to set.
 * @return Always returns 1.
 */
int	expand_set_state(t_quote_state *state, size_t *i, t_quote_state new_state)
{
	*state = new_state;
	(*i)++;
	return (1);
}

/**
 * @brief Initializes an expansion result string and handles tilde expansion.
 *
 * If the input starts with '~' (and is alone or followed by '/'),
 * replaces it with the value of $HOME and any suffix.
 *
 * @param input The input string to expand.
 * @param i     Pointer to the current parsing index (may be incremented).
 * @param shell The shell context for environment access.
 * @return Newly allocated result string, or NULL on allocation failure.
 */
char	*expand_init_result(const char *input, size_t *i, t_shell *shell)
{
	char	*result;
	char	*home;
	char	*suffix;
	char	*joined;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	if (input[0] == '~' && (input[1] == '/' || input[1] == '\0'))
	{
		home = get_env_value("HOME", shell->env);
		if (!home)
			home = "";
		if (input[1])
			suffix = ft_strdup(input + 1);
		else
			suffix = ft_strdup("");
		if (!suffix)
			return (free(result), NULL);
		joined = ft_strjoin(home, suffix);
		*i += 1;
		return (free(suffix), free(result), joined);
	}
	return (result);
}

/**
 * @brief Extracts the variable name and retrieves its value
 * from the environment.
 * 
 * @param ctx  Expander context.
 * @param j    Current parsing index (start of the variable name).
 * @param next_pos Output: updated parsing index after the variable.
 * @return Allocated string of variable value (never NULL string),
 * or NULL if allocation failed.
 */
char	*get_variable_value(t_expander *ctx, size_t j, size_t *next_pos)
{
	size_t	start;
	char	*var_name;
	char	*env_val;
	char	*value;

	start = j;
	while (ft_isalnum(ctx->input[j]) || ctx->input[j] == '_')
		j++;
	*next_pos = j;
	var_name = ft_substr(ctx->input, start, j - start);
	if (!var_name)
		return (NULL);
	env_val = get_shell_var_value(var_name, ctx->shell);
	if (env_val)
		value = ft_strdup(env_val);
	else
		value = ft_strdup("");
	free(var_name);
	if (!value)
		return (NULL);
	return (value);
}
