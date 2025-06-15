/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:52:59 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/14 22:12:07 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Checks for syntax errors in the token list.
 * 
 * Detects invalid chaining of logical operators (pipe, and, or)
 * and invalid use of isolated '&' token.
 * 
 * @param t The token to check.
 * @return 1 if a syntax error is found, 0 otherwise.
 */
int	is_syntax_error(t_token *t)
{
	if ((t->type == TOKEN_PIPE || t->type == TOKEN_AND || t->type == TOKEN_OR))
	{
		if (!t->next)
			return (1);
		if (t->next->type == TOKEN_PIPE || t->next->type == TOKEN_AND
			|| t->next->type == TOKEN_OR)
			return (1);
	}
	if (t->type == TOKEN_WORD && t->value && ft_strcmp(t->value, "&") == 0)
		return (1);
	return (0);
}

/**
 * @brief Prints a syntax error message to standard error.
 * 
 * If the token is provided, prints it. Otherwise, defaults to "newline".
 * 
 * @param token The token string causing the error.
 */
void	print_syntax_error(char *token)
{
	ft_putstr_fd(RED "minishell: syntax error near unexpected token!! `", 2);
	if (token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline'", 2);
	ft_putstr_fd(RESET "\n", 2);
}
