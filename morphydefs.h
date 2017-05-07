//
//  morphydefs.h
//  MorPhy2
//
//  Created by mbrazeau on 07/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef morphydefs_h
#define morphydefs_h

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
    //    struct symbset_s*   symboldict;
    struct symbols {
        int numstates;
        char* statesymbols;
    } symbols;
    gap_t               gaphandl;
    union {
        int     asint;
        Mflt    asfloat;
    } score;
    struct mpl_matrix_s*    inmatrix;
} Morphy_t, *Morphyp;

typedef void* Morphy;
#endif /* morphydefs_h */
