/* 
 * treelib.h
 *
 * Header file containing structures, data definitions and function
 * prototypes for treelib
 * 
 */

#ifndef __TREELIB_H__
#define __TREELIB_H__

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

#include "tldefs.h"
/*!
 @discussion This is the interface struct pointer. It's of type void so that 
 external programs and libraries in other languages can interface with it.
 */
typedef void* TLP;
    
/* Function prototypes */
// TODO: Move unexposed functions elsewhere

// Creating, destroying, and basic manipulation of trees
TLtree*	tl_createtree(const int ntaxa);
TLnode*	tl_createnode(void);
int		tl_destroynode(TLnode *n);

// Storing and retrieving a tree's topology
TLtreelist*	tl_createtreelist(int maxtrees);
int			tl_storetopology(const TLtree *t, TLtreelist *trlst);
int			tl_restore_topol(TLtree* t, TLtopol *topol);

// Creating a consensus tree
TLtree*	tl_contree(const TLtreelist *trlist, TLconsens_t contype, int cutoff);

// IO helpers
int		tl_procNewick(const char *nwktree, int ntaxa, TLtopol *t);
char*	tl_writeNewick(const TLtree *t, char *nwktree);

// Helpers
/*!
 @discussion these functions work internally to convert the exposed handle 
 pointer to the external caller.
 */
TLP tl_TLhandle2TLP(TLhandlp TLhandle);
TLhandlp tl_TLP2TLhandle(TLP tlp);

// Interface
TLP		tl_new_TL(void);
int		tl_delete_TL(TLP trl);
    //TLtree*	tl_get_TLtree(int index, TLP trl);
int		tl_set_current_tree(int index, TLP trl);
int		tl_do_contree(TLconsens_t contype, int cutoff, TLP trl);
int		tl_attach_Newick(const char *Newick, TLP trl);
char*	tl_get_Newick(int trnum, TLP trl);
char*	tl_get_contreeNewick(TLconsens_t contype, TLP trl);
int		tl_store_topology(const TLtree *t, TLP trl);
int		tl_rand_topol(TLP trl);
int		alltopologies(TLP trl);
int		tl_same_tree(int tr1, TLP trl1, int tr2, TLP trl2);
int		compattree(int tr1, TLP trl1, int tr2, TLP trl2);
int		removeduplicates(TLP trl);

int     tl_attach_taxon_label(char *taxlabel, TLP tlp);
char*   tl_get_taxon_label(int index, TLP tlp);
int		tl_set_numtaxa(int ntaxa, TLP tlp);
int     tl_get_numtaxa(TLP tlp);
int		tl_set_maxtrees(int maxtrees, TLP trl);
int		tl_set_autoinc(int increment, TLP trl);
int     tl_get_num_trees(TLP tlp);

int     tl_do_traversal(TLP tlp);
int     tl_get_subtree(int index, TLP tlp);
int     tl_break_subtree(int index, TLP tlp);
int     tl_rebranch_subtree(int tgtindex, TLP tlp);
#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* __TREELIB_H__ */

