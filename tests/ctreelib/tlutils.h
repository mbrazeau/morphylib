#ifndef __TL_UTILS__
#define __TL_UTILS__
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_TOKEN_SIZE 100
#define NWKDELIM    "+(,);"

/* Function prototypes */
void*       tl_alloc(size_t size, int setval);
int         tl_push_label(TLlabel *newlab, TLstrlist *strlst);
int         tl_push_new_label(const char* str, TLstrlist *strlst);
int         tl_check_duplicate_label(const char* newlab, const TLstrlist* labels);
int         tl_add_string(char* str, TLstrlist* strlst);
char*       tl_getc(const char *str);
char*       tl_strdup(const char* str);
char*       tl_int_to_str(long i);
char*       tl_strcat(char** str1, char* str2, bool delete2);
int         tl_get_label_index(const TLlabel *label);
int         tl_calc_nnodes(const int ntaxa);
int         tl_num_nodes_vertices(const int ntaxa);
int         tl_lookup_label(int asint, char* aslabel, TLhandlp handl);
char*       tl_str_append_char(char **str, const char c);

#endif

