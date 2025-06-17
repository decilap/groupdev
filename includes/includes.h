/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:23:30 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/17 13:29:16 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include "../libft/includes/libft.h"
# include "minishell.h"
# include "execution.h"
# include "wildcard.h"
# include "builtins.h"
# include "env.h"
# include "expander.h"
# include "here_doc.h"
# include "lexer.h"
# include "parser.h"
# include "quote.h"
# include "color.h"
# include "history.h"
# include "signals.h"
# include "subshell.h"
# include "token_utils.h"
# include "utils.h"
# include "brain.h"

extern volatile sig_atomic_t	g_signal;

#endif
