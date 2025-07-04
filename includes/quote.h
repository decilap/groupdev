/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:45 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/20 20:35:29 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H

int		handle_token_with_dollar(t_token **curr, t_token **new_tok, t_shell *shell);
int		handle_empty_quote_with_dollar(t_token **curr, t_token **new_tok, t_shell *shell);
int		handle_non_empty_quote_with_dollar(t_token **curr, t_token **new_tok, t_shell *shell);
void	reset_args_and_quotes(t_parse_ctx *ctx, t_shell *shell);

#endif
