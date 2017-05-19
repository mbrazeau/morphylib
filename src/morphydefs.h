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
#define MPLCHARMAX      INT_MAX
    
typedef struct MPLndsets MPLndsets;
typedef struct partition_s MPLpartition;
// Evaluator function pointers
typedef int (*MPLdownfxn)
            (MPLndsets*     lset,
             MPLndsets*     rset,
             MPLndsets*     nset,
             MPLpartition*  part);

typedef int (*MPLupfxn)
            (MPLndsets*     lset,
             MPLndsets*     rset,
             MPLndsets*     nset,
             MPLndsets*     ancset,
             MPLpartition*  part);
    
typedef int (*MPLtipfxn)
            (MPLndsets*     tset,
             MPLndsets*     ancset,
             MPLpartition*  part);

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
    
} MPLgap_t;

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
    Mflt         CIndex;
    Mflt         RCIndex;
    Mflt         HIndex;
    Mflt         RetIndex;
    
} MPLcharinfo;
    
    
typedef struct partition_s MPLpartition;
typedef struct partition_s {
    
    MPLchtype       chtype;
    bool            isNAtype;
    int             maxnchars;
    int             ncharsinpart;
    int*            charindices;
    MPLtipfxn       tipupdate;
    MPLtipfxn       tipfinalize;
    MPLdownfxn      inappdownfxn;
    MPLupfxn        inappupfxn;
    MPLdownfxn      prelimfxn;
    MPLupfxn        finalfxn;
    MPLpartition*   next;
    
} MPLpartition;
    


typedef struct MPLndsets {
    
    MPLstate*   downpass1;
    MPLstate*   uppass1;
    MPLstate*   downpass2;
    MPLstate*   uppass2;
    MPLstate*   subtree_actives;
    MPLstate*   subtree_downpass1;
    MPLstate*   subtree_uppass1;
    MPLstate*   subtree_downpass2;
    MPLstate*   subtree_uppass2;
    char**      downp1str;
    char**      downp2str;
    char**      upp1str;
    char**      upp2str;
    
} MPLndsets;
    
typedef struct mpl_matrix_s {
    int             ncells;
    MPLcell*        cells;
} MPLmatrix;

//typedef struct {
//    int*            tips;
//    int*            internals;
//    MPLndsets**  ancstates;
//} MPLnodesets;

typedef struct symbols_s {
    int         numstates;
    char*       statesymbols;
    char*       symbolsinmatrix;
    MPLstate*   packed;
    char        gap;
    char        missing;
} MPLsymbols;
 
    
/*! \struct*/
typedef struct Morphy_t {
    
    int             numtaxa;    // The number of terminal taxa
    int             numcharacters;  // The number of characters (transformation series)
    MPLcharinfo*    charinfo;   // Data type information about each character
    int             numparts;   // The number of data type partitions
    MPLpartition*   partstack;  // A place for unused partitions
    MPLpartition**  partitions; // The array of partitions
    MPLsymbols      symbols;    // The symbols used in the dataset
    MPLgap_t           gaphandl;   // The method of gap treatment
    union {
        int         asint;
        Mflt        asfloat;
    } score;   // The score (parsimony, likelihood etc.) of the evaluated data
    MPLmatrix       inmatrix;   // Internal representation of the matrix
    char*           char_t_matrix;  // The matrix as a NULL-terminated string
    int             numnodes;   // The number of nodes
    int*            nodesequence;   // The postorder sequence of nodes.
    int             nthreads;   // For programs that wish to multithread
    MPLndsets**     statesets;
    
} Morphy_t, *Morphyp;

typedef void* Morphy;

#ifdef __cplusplus
}
#endif
    
#endif /* morphydefs_h */
