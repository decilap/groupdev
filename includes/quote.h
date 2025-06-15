/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:30:49 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H

int	handle_token_with_dollar(t_token **curr, t_token **new_tok);
int	handle_empty_quote_with_dollar(t_token **curr, t_token **new_tok);
int	handle_non_empty_quote_with_dollar(t_token **curr, t_token **new_tok);
int	handle_dollar_in_double_quote(t_token **curr, t_token **new_tok);
int	handle_expansion_with_dollar(t_token **curr, t_token **new_tok,
		t_shell *shell);
int	handle_case_group1(t_token **curr, t_token **new_tok, t_shell *shell);
int	handle_case_group2(t_token **curr, t_token **new_tok, t_shell *shell);
int	handle_case_group3(t_token **curr, t_token **new_tok);

#endif
