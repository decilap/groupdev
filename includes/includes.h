/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:23:30 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/15 01:57:01 by decilapdeni      ###   ########.fr       */
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
