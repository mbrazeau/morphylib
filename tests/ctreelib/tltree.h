//
//  tltree.h
//  treelib
//
//  Created by mbrazeau on 17/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef tltree_h
#define tltree_h

TLtree*     tl_new_TLtree(int ntaxa);
void        tl_destroy_TLtree(TLtree* tree);
TLtopol*    tl_create_topology_rec(int numtaxa);
int         tl_destroy_topology_rec(TLtopol *oldtp);
int         tl_push_new_topol(TLtopol* topol, TLtreelist* trlist);
TLtopol*    tl_get_topol_from_list(int index, TLtreelist* trlist);
int         tl_attept_topol_push(TLtopol* topol, TLtreelist* trlist);
int         tl_push_topol(TLtopol* new, TLtreelist* trlist);
TLtopol*    tl_pop_topol(TLtreelist* trlist);
TLtopol*    tl_get_topol(int index, TLtreelist* trlist);
int         tl_traverse_tree(TLnode *n, int* index, int* postorder);
void        tl_mark_downpass(TLnode* n, int index, bool* found);
void        tl_reroot_binnode(TLnode* n);
int         tl_root_tree(TLtree* t, int tgtroot, bool polybase);
void        tl_mark_nodes(TLnode* n, int* index, TLtopol* topol);
TLnode*     tl_find_anytip(TLnode* n);
void        tl_extract_internal_node(TLnode* n);
void        tl_quick_root(TLtree* t);
int         tl_unroot_tree(TLtree* t);
void        tl_mark_for_comparison(TLtree* t, TLtopol* topol);
int         tl_compare_topols(TLtopol* tp1, TLtopol* tp2);
int         tl_push_node(const TLnode *n, TLndstack *ndstk);
TLnode*     tl_pull_node(TLndstack *ndstk);
TLnode*     tl_remove_branch(TLnode* n, TLtree* t);
int         tl_insert_branch(TLnode* src, int tgt, TLtree* t);
void        tl_create_starting_fork(int left, int right, int anc, TLtree* t);

#endif /* tltree_h */
