//
//  statedata.h
//  MorPhy2
//
//  Created by mbrazeau on 26/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef statedata_h
#define statedata_h

#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static const char* gmpl_valid_symb =
"\n\t[](){}; -+?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char* gmpl_valid_state =
"+0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char* gmpl_valid_matrix_wildcard = "-?";
static const char* gmpl_valid_matrix_punc = "{}();";

typedef struct {
    MPLstate    asint;
    char*       asstr;
} MPLcell;

typedef struct {
    int         maxnchars;
    int         nchars;
    int*        charindices;
    MPLstate*    cdata;
} MPLpartinfo;

typedef struct {
    int*        charindices;
    MPLstate*   NApass1;
    MPLstate*   NApass2;
    MPLstate*   prelimset;
    MPLstate*   finalset;
    MPLstate*   subtree_NApass1;
    MPLstate*   subtree_NApass2;
    MPLstate*   subtree_prelimset;
    MPLstate*   subtree_finalset;
} MPLancstates;

typedef struct mpl_matrix_s {
    int*        intweights;
    Mflt*       fltweights;
    int         ncells;
    MPLcell*    cells;
} MPLmatrix;

typedef struct {
    int*            tips;
    int*            internals;
    MPLancstates*   ancstates;
} MPLnodesets;

/* Function prototypes */
int         mpl_init_symbolset(Morphyp m);
int         mpl_set_numsymbols(int numsymb, Morphyp handl);
int         mpl_get_numsymbols(Morphyp handl);
void        mpl_destroy_symbolset(Morphyp m);
char*       mpl_skip_closure(const char *closure, const char openc, const char closec);
int         mpl_compare_symbol_lists(const char* sym1, const char* sym2);
int         mpl_assign_symbol_list_from_matrix(const char *symbs, struct MPLsymbols* symlist);
char*       mpl_query_symbols_from_matrix(Morphyp m);
int         mpl_get_states_from_rawdata(Morphyp handl);
int         mpl_copy_raw_matrix(const char* rawmatrix, Morphyp handl);
int         mpl_check_nexus_matrix_dimensions(char *input_matrix, int input_num_taxa, int input_num_chars);
char*       mpl_get_preprocessed_matrix(Morphyp handl);
int         mpl_convert_rawdata(Morphyp handl);
MPLmatrix*  mpl_new_mpl_matrix(const int ntaxa, const int nchar, const int nstates);
int         mpl_delete_mpl_matrix(MPLmatrix* m);
MPLmatrix*  mpl_get_mpl_matrix(Morphyp m);

#endif /* statedata_h */
