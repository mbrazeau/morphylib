//
//  tldefs.h
//  treelib
//
//  Created by mbrazeau on 01/06/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef tldefs_h
#define tldefs_h

#ifdef DEBUG
#include <stdio.h>
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

#include <assert.h>
#include <stdbool.h>
#include <math.h>
// Forward declarations for self-referencing struct typedefs
typedef struct __node_s		TLnode;
typedef struct __comprtre_s	TLtopol;

// Compiler flag can be used to choose floating point type for storing branch
//  lengths and likelihoods in nodes and trees
#ifdef TLDBL
typedef double	TLflt;
#elif TLLDBL
typedef long double TLflt;
#else
typedef float	TLflt;
#endif

#define DEFAULT_MAXTREES    100
#define DEFAULT_AUTOINC     0

#define I_NODE(n, t)       (t->trnodes[n]->index)
#define I_LDESC(n, t)      (t->trnodes[n].left->index)
#define I_RDESC(n, t)      (t->trnodes[n].right->index)
#define I_NEXTDESC(n, t)   (t->trnodes[n].sib->index)
#define I_ANCESTOR(n, t)   (t->trnodes[n].anc->index)


#define tl_get_TLtree(trl)  (((TLhandlp)trl)->tree)
#define tl_num_nodes(t)     (t->nnodes)
#define tl_dummy_root(t)    (t->trnodes[t->nnodes])
#define tl_upper_root(t)    (t->trnodes[t->nnodes-1])
    
typedef struct __node_s {
	char	*label;     /*!< Tip label (taxon name) */
    int     tip;        /*!< Tip number (shorthand for name) */
	int		index;      /*!< Memory index of the node */
	int		weight;     /*!< Total number of descendant tips */
    bool    inpath;     /*!< Used for marking a traversal path in tree */
    int     downpmark;  /*!< Used for describing topology as int array */
    bool    ispolynode; /*!< Shorthand for polytomy node */
	TLflt	length;     /*!< Length of the branch subtending this node */
    TLnode* left;       /*!< Pointer to left descendant node */
    TLnode* right;      /*!< Pointer to right descendant node in binary trees */
    TLnode* sib;        /*!< Pointer to sibling in descendant of a polytomous node*/
    TLnode* anc;        /*!< Pointer to the parent (ancestor) node */
} TLnode;

// Storage for allocated nodes not in use in the core tree structure
typedef struct {
	int		nnodes;
	int		maxnodes;
	TLnode	**stack;
	TLnode	*head;
} TLndstack;


typedef struct {
    int     travmax;
    int     end;
    int     current;
    int*    travindices;
} TLtravlist;


typedef struct {
    int     n_srcnodes;
    int*    srcindices;
    int     n_tgtnodes;
    int*    tgtindices;
} TLbrswap;

   
/*!
 The core tree structure in the TLP object.
 */
typedef struct {
    int         nnodes;
	TLnode*     trnodes;
	TLndstack	nodestk;
	TLnode*     start;
    int         npolynodes;
    TLnode**    polynodes;
    TLbrswap    brswap;
    int*        postorder;
	int			isrooted;
	int			ntaxa;
	int			parscore;
	TLflt		likelihood;
} TLtree;


/*!
 Basic storage for topological information of a tree. Topology data is stored as
 an array of indices indicating the node that the ith node is connected to.
 Other storage types, such as Newick string, can be stored here as well.
 */
typedef struct __comprtre_s {
	int			nnodes;
    bool        rooted;
    int         nvertices;
	int			*edgeIDs;
    int         *postord_i;
	char		*Newick;
	int			parscore;
    TLnode*     edges;
    TLnode*     nexts;
	TLflt		likelihood;
	TLtopol*    next;
    TLtopol*    back;
	TLflt		*brlens;
} TLtopol;

// Consensus tree types
typedef enum {
	NONE,
	STRICT_CT,
	SEMISTRICT_CT,
	MAJRULE_CT,
	ADAMS_CT,
	CONTREE_MAX,
} TLconsens_t;

/*!
 The list structure that stores the trees.
 */
typedef struct __treelist_s {
	int			ntrees;
	int			maxtrees;
    int         autoinc;
	TLtopol		*first;
	TLtopol		*last;
	TLtopol		**trees;
	TLtopol		**contree;
	TLconsens_t	*contreetypes[CONTREE_MAX];
} TLtreelist;


/*!
 Cluster table for consensus methods
 */
typedef struct {
	int *external;
	int *internal;
	int *weight;
} TLclustable;


typedef struct {
    int             nodeindex;
    unsigned long   labellen;
    char            *label;
} TLlabel;

/*!
 Generic structure for a list of strings. This would store, for instance, the 
 set of node labels supplied by the user.
 */
typedef struct {
	int     nelems;
    int     maxelems;
    TLlabel **labels;
} TLstrlist;

/*!
 The central data structure of treelib. The user creates an instance of this 
 structure to begin a treelib session. This user interacts with this structure
 only through library functions.
 */
typedef struct __tl_handle_s {
    struct {
        int ntaxa;
    } dimensions;
	TLstrlist	taxlabels;
	TLtree		*tree;
	TLtreelist	treelist;	
} TLhandl, *TLhandlp;


#endif /* tldefs_h */
