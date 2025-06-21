/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <ddecilap@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:57:34 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/21 12:39:29 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Librairies standard nécessaires
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <glob.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stddef.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <limits.h>

# define WILDCARD_CAPACITY 64

# define ERROR -1
# define SUCCESS 0

# define TOKEN_WORD      0
# define TOKEN_PIPE      1
# define TOKEN_REDIR_IN  2
# define TOKEN_REDIR_OUT 3
# define TOKEN_APPEND    4
# define TOKEN_HEREDOC   5
# define TOKEN_AND   6
# define TOKEN_OR   7
# define TOKEN_POINT_VIRGULE   8
# define TOKEN_NONE   9
# define TOKEN_SUBSHELL  10
# define MAX_SUBSHELLS 4096
# define MAX_PIPE_CMDS 4096
# define MAX_CMD_ARGS 4096

# define MINI_PATH_MAX        4096

# define ERR_OPEN "minishell: Unable To Open Or Create History File And Retriev\
e Data"

/**
 * @brief Enumeration of possible quoting states during parsing/expansion.
 *
 * Used to track whether the parser is outside quotes, inside single quotes,
 * or inside double quotes.
 */
typedef enum e_quote_state
{
	Q_NONE,
	Q_SINGLE_QUOTE,
	Q_DOUBLE_QUOTE,
	Q_HEREDOC
}	t_quote_state;

/**
 * @brief Linked list node for shell history entries.
 *
 * Stores a command line and a pointer to the next history entry.
 */
typedef struct s_history
{
	char				*cmd;
	struct s_history	*next;
}	t_hist;

/**
 * @brief Represents a single parsed command in the shell.
 *
 * Contains arguments, resolved executable path, file descriptors for input/
 * output,
 * information about redirections, and links to the next command in the chain.
 */
typedef struct s_cmd
{
	char			**args;
	char			*cmd_path;
	int				fd_in;
	int				fd_out;
	int				redir;
	int				next_type;
	int				prev_type;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	t_quote_state	*quote_chars;
}	t_cmd;

/**
 * @brief Represents a key-value pair for shell variables.
 *
 * Used for storing both environment and local variables as a linked list.
 */
typedef struct s_var
{
	char			*key;
	char			*value;
	struct s_var	*next;
}	t_var;

/**
 * @brief Global state/context for the shell session.
 *
 * Stores environment, exit status, variables, history, and other global state.
 */
typedef struct s_shell
{
	char			**env;
	int				exit_status;
	t_var			*locals;
	int				last_op;
	int				syntax_error_printed;
	pid_t			pid;
	t_hist			*history;
	size_t			histfile_len;
	size_t			hist_len;
	struct s_cmd	*cmds;
}	t_shell;

/**
 * @brief Represents a token produced by lexical analysis.
 *
 * Contains the token type, quoting information, value, and links to the next
 * token.
 */
typedef struct s_token
{
	int				type;
	int				quoted;
	int				joined;
	char			*value;
	t_quote_state	quote_char;
	struct s_token	*next;
}	t_token;

/**
 * @brief Represents a group of commands joined by logical operators (&&, ||).
 *
 * Used for handling logical chaining and prioritization in the shell parser/
 * executor.
 */
typedef struct s_group
{
	int				next_op;
	t_cmd			*cmds;
	struct s_group	*next;
}	t_group;

void	clean_exit(t_shell *shell);

#endif
