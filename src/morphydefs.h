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
#include <stdbool.h>
#ifdef MPLDBL
typedef double Mflt;
#elif MPLLDBL
typedef long double Mflt;
#else
typedef float Mflt;
#endif

typedef unsigned int MPLstate;

#define NA              ((MPLstate)1)
#define MISSING         ((MPLstate)~0)
#define ISAPPLIC        (((MPLstate)~0)^NA)
#define MAXSTATES       (CHAR_BIT * sizeof(MPLstate))
#define DEFAULTGAP      '-'
#define DEFAULTMISSING  '?'
#define DEFAULCHARTYPE  FITCH_T
#define NACUTOFF        2   // The max number of NA tokens that can be ignored
                            // in a column
    

// Evaluator function pointers
typedef int (*MPLdownfxn)
(MPLstate* left, MPLstate* right, MPLstate* n, int nchars);

typedef int (*MPLupfxn)
(MPLstate* left, MPLstate* right, MPLstate* n, MPLstate *anc, int nchars);

// Key data types
typedef enum {
    
    NONE_T          = 0,
    FITCH_T         = 1,
    WAGNER_T        = 2,
    DOLLO_T         = 3,
    IRREVERSIBLE_T  = 4,
    USERTYPE_T      = 5,
    
    MAX_CTYPE,
    
} MPLchtype;

typedef enum {
    
    GAP_INAPPLIC,
    GAP_MISSING,
    GAP_NEWSTATE,
    
    GAP_MAX,
    
} gap_t;

typedef struct {
    MPLstate    asint;
    char*       asstr;
} MPLcell;
    

typedef struct charinfo_s MPLcharinfo;
typedef struct charinfo_s {
    
    int          charindex;
    int          ninapplics;
    bool         included;
    MPLchtype    chtype;
    union {
        int      intwt;
        Mflt     fltwt;
    };
    
} MPLcharinfo;
    
    
typedef struct partition_s MPLpartition;
typedef struct partition_s {
    
    MPLchtype       chtype;
    bool            isNAtype;
    int             maxnchars;
    int             ncharsinpart;
    int*            charindices;
    MPLdownfxn      inappdownfxn;
    MPLupfxn        inappupfxn;
    MPLdownfxn      prelimfxn;
    MPLupfxn        finalfxn;
    MPLpartition*   next;
    
} MPLpartition;
    


typedef struct {
    
    MPLstate*   NApass1;
    MPLstate*   NApass2;
    MPLstate*   prelimset;
    MPLstate*   finalset;
    MPLstate*   subtree_actives;
    MPLstate*   subtree_NApass1;
    MPLstate*   subtree_NApass2;
    MPLstate*   subtree_prelimset;
    MPLstate*   subtree_finalset;
    
} MPLstatesets;
    
typedef struct mpl_matrix_s {
    int             ncells;
    MPLcell*        cells;
} MPLmatrix;

typedef struct {
    int*            tips;
    int*            internals;
    MPLstatesets**  ancstates;
} MPLnodesets;

typedef struct symbols_s {
    int         numstates;
    char*       statesymbols;
    char*       symbolsinmatrix;
    MPLstate*   packed;
    char        gap;
    char        missing;
} MPLsymbols;
    
typedef struct __morphy_s {
    
    int             numtaxa;
    int             numcharacters;
    MPLcharinfo*    charinfo;
    int             numschtypes[MAX_CTYPE];
    int             numwNAchtypes[MAX_CTYPE];
    int             numparts;
    MPLpartition*   partinfo;
    MPLsymbols      symbols;
    gap_t           gaphandl;
    union {
        int         asint;
        Mflt        asfloat;
    } score;
    MPLmatrix       inmatrix;
    char*           char_t_matrix;
    int             numnodes;
    int*            nodesequence;
    
} Morphy_t, *Morphyp;

typedef void* Morphy;

#ifdef __cplusplus
}
#endif
    
#endif /* morphydefs_h */
