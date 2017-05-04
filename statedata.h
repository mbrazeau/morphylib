//
//  statedata.h
//  MorPhy2
//
//  Created by mbrazeau on 26/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef statedata_h
#define statedata_h

#include <stdint.h>
#include <limits.h>
#include <string.h>

typedef uint64_t Mstates;

#define ISAPPLIC    (UINT64_MAX - 1)
#define NA          1
#define MISSING     UINT64_MAX

#define MAXSTATES   (CHAR_BIT * sizeof(Mstates))

static const char* gmpl_valid_symb =
"\n\t[]{} -+?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char* gmpl_valid_state =
"-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

typedef struct{
    Mstates asint;
    char    aschar;
} MPL_stsymb;

typedef struct symbset_s {
    int         numstates;
    char*       rawsymbols;
    MPL_stsymb* symbols;
} MPL_symbset;

typedef struct {
    int         maxnchars;
    int         nchars;
    int*        charindices;
    Mstates*    cdata;
} MPLpartition;

typedef struct {
    int         nchars;
    int*        charindices;
    Mstates*    NApass1;
    Mstates*    NApass2;
    Mstates*    prelimset;
    Mstates*    finalset;
    Mstates*    subtree_NApass1;
    Mstates*    subtree_NApass2;
    Mstates*    subtree_prelimset;
    Mstates*    subtree_finalset;
} MPLancstates;

typedef struct {
    int         numrows;
    int         numcols;
    char*       rawdata;
    Mstates*    tipdata;
    Mstates**   nodalstates;
} MPLmatrix;

typedef struct {
    int*            tips;
    int*            internals;
    MPLancstates*   ancstates;
} MPLnodesets;

/* Function prototypes */
int             mpl_get_states_from_rawdata(Morphyp handl);
void            mpl_convert_rawdata(Morphyp handl);
MPL_symbset*    mpl_alloc_symbolset(void);
void            mpl_destroy_symbolset(MPL_symbset* symbs);

#endif /* statedata_h */
