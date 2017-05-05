//
//  morphy.h
//  MorPhy2
//
//  Created by mbrazeau on 23/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef morphy_h
#define morphy_h

#ifdef DEBUG
#include <stdio.h>
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include <glib.h>

#ifdef MDBL
typedef double Mflt;
#elif MLDBL
typedef long double Mflt;
#else
typedef float Mflt;
#endif

typedef enum {
    FITCH_T,
    WAGNER_T,
    DOLLO_T,
    IRREVERSIBLE_T,
    USERTYPE_T,
    MAX_CTYPE,
} ptype_t;

typedef enum {
    GAP_INAPPLIC,
    GAP_MISSING,
    GAP_NEWSTATE,
    GAP_MAX,
} gap_t;

typedef struct __morphy_s {
    int                 numtaxa;
    int                 numcharacters;
    char*               char_t_matrix;
    int                 numnodes;
    int*                nodesequence;
    struct symbset_s*   symboldict;
    gap_t               gaphandl;
    union {
        int     asint;
        Mflt    asfloat;
    } score;
    struct mpl_matrix_s*    inmatrix;
} Morphy_t, *Morphyp;

/* Function prototypes */
Morphyp mpl_new_Morphy_t(void);
void*   mpl_get_from_matrix(const int row, const int col, const size_t size, const void* data);
int     mpl_get_gaphandl(Morphyp handl);
#endif /* morphy_h */
