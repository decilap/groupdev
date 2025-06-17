#include "../../includes/includes.h"

#include <stdlib.h>
#include <stdio.h>


int g_malloc_fail = 0;  // Compteur global pour contrôler les échecs au 'x'eme appele de malloc on a un malloc NULL.

// ne pas normé ne pas garder projet fini , egale test

void *my_malloc(size_t size)
{

    static int count = 1;
    if (g_malloc_fail && count++ >= g_malloc_fail) 
	{
        fprintf(stderr, "Simulated malloc failure!\n");
        return NULL;
    }


    return malloc(size);
}