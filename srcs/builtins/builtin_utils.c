/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:39:47 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/17 13:25:22 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h" 

/**
 * @brief Vérifie si une commande est un built-in de minishell.
 *
 * Cette fonction teste si le premier argument correspond à l'un des built-ins
 * reconnus par le shell.
 *
 * @param cmd Pointeur vers la structure de commande.
 * @return 1 si c'est un built-in, 0 sinon.
 */
int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	return (ft_strcmp(cmd->args[0], "echo") == 0
		|| ft_strcmp(cmd->args[0], "cd") == 0
		|| ft_strcmp(cmd->args[0], "pwd") == 0
		|| ft_strcmp(cmd->args[0], "export") == 0
		|| ft_strcmp(cmd->args[0], "unset") == 0
		|| ft_strcmp(cmd->args[0], "env") == 0
		|| ft_strcmp(cmd->args[0], "exit") == 0
		|| ft_strcmp(cmd->args[0], "history") == 0
	);
}

/**
 * @brief Vérifie si une chaîne correspond à l'option `-n` de echo.
 *
 * Cette fonction teste si la chaîne commence par `-n` et est suivie
 * uniquement de 'n'.
 *
 * @param arg Chaîne à vérifier.
 * @return 1 si la chaîne est composée uniquement de `-n`, 0 sinon.
 */
static int	is_all_n(const char *arg)
{
	int	j;

	j = 1;
	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (0);
		j++;
	}
	return (1);
}

/**
 * @brief Analyse les options `-n` de la commande `echo`.
 *
 * Cette fonction parcourt les arguments pour détecter toutes les options `-n`
 * et modifie le comportement de saut de ligne si besoin.
 *
 * @param args Tableau d'arguments de echo.
 * @param newline Pointeur vers un booléen qui désactive l'ajout du newline
 * si `-n` est présent.
 * @return L'indice à partir duquel commencent les vrais arguments à afficher.
 */
int	parse_echo_flags(char **args, int *newline)
{
	int	i;

	i = 1;
	while (args[i] && is_all_n(args[i]))
	{
		*newline = 0;
		i++;
	}
	return (i);
}

/**
 * @brief Copie et trie l'environnement en ordre ASCII.
 *
 * @param env L'environnement d'origine.
 * @return Un tableau nouvellement alloué trié, ou NULL si erreur.
 */
char	**prepare_sorted_env(char **env)
{
	int		count;
	int		i;
	char	**sorted_env;

	count = 0;
	while (env[count])
		count++;
	sorted_env = mallocsizeof(char *) * (count + 1));
	if (!sorted_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted_env[i] = ft_strdup(env[i]);
		i++;
	}
	sorted_env[count] = NULL;
	sort_ascii(sorted_env, count);
	return (sorted_env);
}
