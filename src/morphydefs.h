//
//  morphydefs.h
//  MorPhy2
//
//  Created by mbrazeau on 07/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef morphydefs_h
#define morphydefs_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef MDBL
typedef double Mflt;
#elif MLDBL
typedef long double Mflt;
#else
typedef float Mflt;
#endif

typedef uint64_t MPLstate;

#define ISAPPLIC        (UINT64_MAX - 1)
#define NA              ((MPLstate)1)
#define MISSING         ((MPLstate)UINT64_MAX)
#define MAXSTATES       (CHAR_BIT * sizeof(MPLstate))
#define DEFAULTGAP      '-'
#define DEFAULTMISSING  '?'

typedef enum {
    FITCH_T,
    WAGNER_T,
    DOLLO_T,
    IRREVERSIBLE_T,
    USERTYPE_T,
    MAX_CTYPE,
} MPLptype_t;

typedef enum {
    GAP_INAPPLIC,
    GAP_MISSING,
    GAP_NEWSTATE,
    GAP_MAX,
} gap_t;

typedef struct __morphy_s {
    
    int         numtaxa;
    int         numcharacters;
    char*       char_t_matrix;
    int         numnodes;
    int*        nodesequence;
    
    struct MPLsymbols {
        int         numstates;
        char*       statesymbols;
        char*       symbolsinmatrix;
        MPLstate*   packed;
        char        gap;
        char        missing;
    } symbols;
    
    gap_t       gaphandl;
    
    union {
        int     asint;
        Mflt    asfloat;
    } score;
    
    struct mpl_matrix_s* inmatrix;
    
} Morphy_t, *Morphyp;

typedef void* Morphy;

#ifdef __cplusplus
}
#endif
    
#endif /* morphydefs_h */
