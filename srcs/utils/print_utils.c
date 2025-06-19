/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: decilapdenis <decilapdenis@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:09:31 by ryoussfi          #+#    #+#             */
/*   Updated: 2025/06/19 15:00:58 by decilapdeni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.h"

/**
 * @brief Prints the environment variables.
 * 
 * @param env The array of environment variables.
 */
/* void	print_env(char **env)
{
	int		i;
	char	*index_str;

	i = 0;
	while (env[i])
	{
		index_str = ft_itoa(i);
		ft_putstr_fd("[", STDOUT_FILENO);
		ft_putstr_fd(index_str, STDOUT_FILENO);
		ft_putstr_fd("] ", STDOUT_FILENO);
		ft_putendl_fd(env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		free(index_str);
		i++;
	}
} */


/**
 * @brief Prints the list of tokens.
 * 
 * @param tok The list of tokens to print.
 */
/* void print_tokens(t_token *tok)
{
	const char *qtype;
	while (tok)
	{
		if (tok->quote_char == Q_SINGLE_QUOTE)
			qtype = "'";
		else if (tok->quote_char == Q_DOUBLE_QUOTE)
			qtype = "\"";
		else
			qtype = "NONE";

		fprintf(stderr, "token: value='%s' | quoted=%d
			| quote_char=%s | joined=%d | type=%d\n",
				tok->value ? tok->value : "(null)",
				tok->quoted,
				qtype,
				tok->joined,
				tok->type);
		tok = tok->next;
	}
}
 */

/**
 * @brief Prints the list of commands.
 * 
 * @param cmd The list of commands to print.
 */
/* void	print_cmd(t_cmd *cmd)
{
	int	i;

	fprintf(stderr, "--- Début de l'affichage des commandes ---\n");
	while (cmd)
	{
		fprintf(stderr, "CMD à l'adresse %p:\n", (void *)cmd);
		i = 0;
		if (cmd->args)
		{
			while (cmd->args[i])
			{
				fprintf(stderr, " - arg[%d]: '%s'\n", i, cmd->args[i]);
				i++;
			}
		}
		else
		{
			fprintf(stderr, " - args: (null)\n");
		}
		fprintf(stderr, " - fd_in: %d, fd_out: %d\n", cmd->fd_in, cmd->fd_out);
		const char *next_type_str = "INCONNU";
		if (cmd->next_type == 0) next_type_str = "CMD_END / NONE"; 
		else if (cmd->next_type == 1) next_type_str = "PIPE"; 
		else if (cmd->next_type == 2) next_type_str = "AND";
		else if (cmd->next_type == 3) next_type_str = "OR";
		fprintf(stderr, " -> next_type: %d (%s), next: %p\n",
			cmd->next_type, next_type_str, (void *)cmd->next);
		cmd = cmd->next;
		if (cmd)
			fprintf(stderr, "--------------------------------------\n");
	}
	fprintf(stderr, "--- Fin de l'affichage des commandes ---\n");
} */


/**
 * @brief Prints the list of groups and their commands.
 * 
 * @param g The list of groups to print.
 */
/* void	print_groups(t_group *g)
{
	int		i;
	t_cmd	*c;

    // Use fprintf(stderr, ...) for debug output
    fprintf(stderr, "--- DEBUG: Entering print_groups ---\n");
    i = 1;
    while (g)
    {
        fprintf(stderr, "Group %d (Address: %p):\n", i++, (void *)g);
        c = g->cmds;
        if (c) // Check if there are commands in the group
        {
            while (c)
            {
                fprintf(stderr, "  Command: '%s' (Args: %s, Next Cmd: %p)\n",
                        c->args ? c->args[0] : "(null)",
                        c->args && c->args[0] ? c->args[1] ? c->args[1]
						: "(no further args)" : "(no args)",
                        (void *)c->next);
                c = c->next;
            }
        }
        else
        {
            fprintf(stderr, "  (No commands in this group)\n");
        }
        
        // Map next_op to a readable string
        const char *op_str = "UNKNOWN";
        if (g->next_op == 0) op_str = "NONE / END_OF_LINE";
        else if (g->next_op == 1) op_str = "PIPE";
        else if (g->next_op == 2) op_str = "AND";
        else if (g->next_op == 3) op_str = "OR";
        // Add more if you have other operators

        fprintf(stderr, "  Next operator (logical): %d (%s), Next Group: %p\n",
                g->next_op, op_str, (void *)g->next);
        
        g = g->next;
        if (g) // Add a separator if there are more groups
            fprintf(stderr, "----------------------------------------\n");
    }
    fprintf(stderr, "--- DEBUG: Exiting print_groups ---\n");
}
 */