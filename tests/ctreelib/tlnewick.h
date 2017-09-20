//
//  tlnewick.h
//  treelib
//
//  Created by mbrazeau on 08/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef tlnewick_h
#define tlnewick_h

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NWKROOTCMD "[&"
#define NWKTKNSYMB " _\'\""

typedef struct {
    unsigned int maxlen;
    unsigned int len;
    char* Newick_string;
} TLnewick;

char*   tl_read_Newick_brlen(TLflt *brlen, const char* str);
char*   tl_read_token(char* token, const char* str);
int     tl_recurse_Newick(char **nwk, int *index, int* polist, TLhandlp handl);
int     tl_verify_Newick(const char *nwk);
int     tl_read_Newick(const char *nwk, TLhandlp handl);
char*   tl_extract_taxlabel(char *token);
char*   tl_tokenise_Newick(const char *nwk);
int     tl_compr_by_index(const void *ptr1, const void *ptr2);
void    tl_write_Newick_traversal(TLnode *n, bool dolabels, char** result, TLstrlist* labels);
#endif /* tlnewick_h */
