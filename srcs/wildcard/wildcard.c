/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 09:12:01 by ddecilap          #+#    #+#             */
/*   Updated: 2025/06/17 13:42:41 by ryoussfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.h"

/**
 * @brief Initializes the wildcard context for storing expansion results.
 *
 * Allocates the results array and sets the initial capacity and count.
 *
 * @param ctx Pointer to the wildcard context to initialize.
 */
static void	init_wildcard_ctx(t_wildcard_ctx *ctx)
{
	ctx->capacity = WILDCARD_CAPACITY;
	ctx->count = 0;
	ctx->results = malloc(sizeof(char *) * ctx->capacity);
	if (!ctx->results)
		exit_error("malloc( failed in wildcard");
}

/**
 * @brief Grows the results array in the wildcard context
 * when capacity is exceeded.
 *
 * Allocates a new array with double the capacity, copies existing results,
 * and updates the context.
 *
 * @param ctx Pointer to the wildcard context to grow.
 */
static void	wildcard_grow_results(t_wildcard_ctx *ctx)
{
	int		new_cap;
	int		i;
	char	**new_results;

	i = 0;
	new_cap = ctx->capacity * 2;
	new_results = malloc(sizeof(char *) * new_cap);
	if (!new_results)
	{
		free_wildcard_ctx(ctx);
		exit_error("realloc failed in wildcard");
		return ;
	}
	while (i < ctx->count)
	{
		new_results[i] = ctx->results[i];
		i++;
	}
	free(ctx->results);
	ctx->results = new_results;
	ctx->capacity = new_cap;
}

/**
 * @brief Processes a directory entry for wildcard expansion.
 *
 * Ignores hidden files (starting with '.'), matches the pattern, and
 * adds the matching filename to the results array (grows if needed).
 *
 * @param entry   Directory entry to process.
 * @param pattern Pattern to match against.
 * @param ctx     Pointer to the wildcard context.
 */
static void	process_entry(struct dirent *entry,
	const char *pattern, t_wildcard_ctx *ctx)
{
	if ((entry->d_name[0] == '.' && pattern[0] != '.')
		|| ft_strcmp(entry->d_name, ".") == 0
		|| ft_strcmp(entry->d_name, "..") == 0)
		return ;
	if (!match_pattern(pattern, entry->d_name))
		return ;
	if (ctx->count >= ctx->capacity - 1)
		wildcard_grow_results(ctx);
	ctx->results[ctx->count++] = ft_strdup(entry->d_name);
}

/**
 * @brief Expands wildcard matches within a given directory.
 *
 * Opens the directory, processes each entry for pattern matching, sorts results,
 * and returns a NULL-terminated array of matching filenames.
 *
 * @param dirpart  The directory path to search.
 * @param pattern  The wildcard pattern to match.
 * @return NULL-terminated array of matching filenames (malloc('ed),
 * or NULL on error.
 */
static char	**wildcard_expand_dir(const char *dirpart, const char *pattern)
{
	DIR				*dir;
	t_wildcard_ctx	ctx;
	struct dirent	*entry;

	init_wildcard_ctx(&ctx);
	dir = opendir(dirpart);
	if (!dir)
		return (free(ctx.results), NULL);
	entry = readdir(dir);
	while (entry)
	{
		process_entry(entry, pattern, &ctx);
		entry = readdir(dir);
	}
	closedir(dir);
	if (ctx.count == 0)
		return (free(ctx.results), NULL);
	sort_ascii(ctx.results, ctx.count);
	ctx.results[ctx.count] = NULL;
	return (ctx.results);
}

/**
 * @brief Expands a wildcard pattern, supporting optional path components.
 *
 * Parses the input pattern, determines the directory and pattern parts,
 * and calls wildcard_expand_dir. Supports both "./pattern" and "pattern".
 *
 * @param pattern The full pattern to expand (may include directory).
 * @return NULL-terminated array of matching filenames, or NULL on error.
 */
char	**wildcard_expand(const char *pattern)
{
	char	pat[MINI_PATH_MAX];
	char	*slash;
	char	*patternpart;
	char	*dirpart;

	ft_strncpy(pat, pattern, sizeof(pat) - 1);
	pat[sizeof(pat) - 1] = '\0';
	slash = strrchr(pat, '/');
	if (slash)
	{
		*slash = '\0';
		dirpart = pat;
		patternpart = slash + 1;
	}
	else
	{
		dirpart = ".";
		patternpart = pat;
	}
	return (wildcard_expand_dir(dirpart, patternpart));
}
