#include "../includes/includes.h"

#define TMP_FILE "/tmp/.minishell_heredoc_tmp"

static sig_atomic_t *get_heredoc_interrupt_flag(void)
{
	static sig_atomic_t interrupted = 0;
	return (&interrupted);
}

static void sigint_here_doc_handler(int sig)
{
	(void)sig;
	*get_heredoc_interrupt_flag() = 1;
	write(STDOUT_FILENO, "\n", 1);
}

int handle_here_doc(t_shell *shell, t_cmd *cmd, const char *delim_raw, int quoted, t_token *multi_data)
{
	*get_heredoc_interrupt_flag() = 0;
	struct sigaction sa_new;
	sa_new.sa_handler = sigint_here_doc_handler;
	sa_new.sa_flags = 0;
	sigemptyset(&sa_new.sa_mask);
	struct sigaction sa_old;
	sigaction(SIGINT, &sa_new, &sa_old);
    t_token *scan;

	char *clean_delim = ft_strdup(delim_raw);
	if (!clean_delim)
		return (-1);
	if (quoted)
		remove_quotes(&clean_delim);

	int fd_tmp = open(TMP_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd_tmp < 0)
	{
		perror("heredoc: open temp");
		free(clean_delim);
		sigaction(SIGINT, &sa_old, NULL);
		return (-1);
	}

	// 🎤 Mode interactif readline
 	if (multi_data == NULL)
	{ 

		while (!*get_heredoc_interrupt_flag())
		{
			char *line = readline("> ");
			if (!line)
				break;
			if (ft_strcmp(line, clean_delim) == 0)
			{
				free(line);
				break;
			}
			if (!quoted)
			{
				t_token *expanded = expand_variables(line, shell, 1, Q_NONE);
				if (expanded && expanded->value)
				{
					ft_putstr_fd(expanded->value, fd_tmp);
					write(fd_tmp, "\n", 1);
				}
				free_tokens(expanded);
			}
			else
			{
				ft_putstr_fd(line, fd_tmp);
				write(fd_tmp, "\n", 1);
			}
			free(line);
		}
	} 
	else
	{
		scan = multi_data->next;
		while (scan && scan->value)
		{
			if (ft_strcmp(scan->value, clean_delim) == 0)
				break;

			char *to_write = NULL;

			if (!quoted)
			{
				t_token *expanded = expand_variables(scan->value, shell, 1, Q_NONE);
				if (expanded && expanded->value)
					to_write = ft_strdup(expanded->value);
				free_tokens(expanded);
			}
			else
			{
				to_write = ft_strdup(scan->value);
			}

			if (to_write)
			{
				ft_putstr_fd(to_write, fd_tmp);
				write(fd_tmp, "\n", 1);
				free(to_write);
			}

			scan = scan->next;
		}
	}

	close(fd_tmp);
	sigaction(SIGINT, &sa_old, NULL);
	free(clean_delim);

	if (*get_heredoc_interrupt_flag())
	{
		unlink(TMP_FILE);
		shell->exit_status = 130;
		cmd->fd_in = -1;
		return (-1);
	}

	cmd->fd_in = open(TMP_FILE, O_RDONLY);
	unlink(TMP_FILE);
	if (cmd->fd_in < 0)
	{
		perror("heredoc: reopen");
		return (-1);
	}
	return (0);
}
