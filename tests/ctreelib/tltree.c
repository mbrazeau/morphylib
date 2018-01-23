//
//  tltree.c
//  treelib
//
//  Created by mbrazeau on 17/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "tldefs.h"
#include "treelib.h"
#include "tltree.h"
#include "tlerror.h"
#include "tlutils.h"

// TODO: REmove:
#include <stdio.h>
#include <assert.h>

TLtree* tl_new_TLtree(int ntaxa)
{
    TLtree* new = tl_alloc(sizeof(TLtree), 0);
    if (!new) {
        return NULL;
    }
  
    int numnodes = 0;
    numnodes = tl_calc_nnodes(ntaxa);
    
    new->trnodes = (TLnode*)tl_alloc((numnodes+1) * sizeof(TLnode), 0);
    if (!new->trnodes) {
        free(new);
        return NULL;
    }
    
    new->postorder = (int*)tl_alloc(numnodes * sizeof(int), 0);
    if (!new->postorder) {
        tl_destroy_TLtree(new);
        return NULL;
    }
    
    new->nodestk.stack = (TLnode**)tl_alloc(numnodes * sizeof(TLnode*), 0);
    if (!new->nodestk.stack) {
        tl_destroy_TLtree(new);
        return NULL;
    }
    new->nodestk.maxnodes = numnodes;
    new->nodestk.nnodes = 0;
    
    int i = 0;
    for (i = 0; i < numnodes + 1; ++i) {
        
        new->trnodes[i].index = i;
        
        if (i < ntaxa) {
            new->trnodes[i].tip = i+1;
        }
    }
    
    // Setup the dummy root connection
    new->ntaxa  = ntaxa;
    new->nnodes = numnodes;
    
    TLnode* dummy = &tl_dummy_root(new);
    dummy->tip = -1;
    TLnode* uroot = &tl_upper_root(new);
    uroot->anc = dummy;
    dummy->anc = uroot;
    
    return new;
}


void tl_destroy_TLtree(TLtree* tree)
{
    if (tree) {
        if (tree->trnodes) {
            free(tree->trnodes);
            tree->trnodes = NULL;
        }
        if (tree->polynodes) {
            free(tree->polynodes);
            tree->polynodes = NULL;
        }
        if (tree->postorder) {
            free(tree->postorder);
            tree->postorder = NULL;
        }
        
        if (tree->nodestk.stack) {
            free(tree->nodestk.stack);
            tree->nodestk.stack = NULL;
        }
        
        free(tree);
    }
}

TLtopol* tl_create_topology_rec(int numtaxa)
{
    TLtopol *new = NULL;
    
    if (numtaxa) {
        
        new = (TLtopol*)tl_alloc(sizeof(TLtopol), 0);
        new->nnodes     = tl_calc_nnodes(numtaxa);
        new->nvertices  = tl_num_nodes_vertices(numtaxa);
        
        new->edgeIDs    = (int*)tl_alloc(new->nnodes * sizeof(int), 0);
        if (!new->edgeIDs) {
            free(new);
            return NULL;
        }
        
        new->postord_i  = (int*)tl_alloc((numtaxa + new->nvertices)
                                         * sizeof(int), 0);
        new->edgeIDs    = (int*)tl_alloc((numtaxa + new->nvertices)
                                         * sizeof(int), 0);
        if (!new->postord_i) {
            free(new->edgeIDs);
            free(new);
            return NULL;
        }
        
        new->brlens = (TLflt*)tl_alloc(new->nnodes * sizeof(TLflt), 0);
        if (!new->brlens) {
            free(new->postord_i);
            free(new->edgeIDs);
            free(new);
            return NULL;
        }
    }
    
    return new;
}


int tl_destroy_topology_rec(TLtopol *oldtp)
{
    if (oldtp) {
        if (oldtp->brlens) {
            free(oldtp->brlens);
            oldtp->brlens = NULL;
        }
        if (oldtp->postord_i) {
            free(oldtp->postord_i);
            oldtp->postord_i = NULL;
        }
        if (oldtp->edgeIDs) {
            free(oldtp->edgeIDs);
            oldtp->edgeIDs = NULL;
        }
        if (oldtp->Newick) {
            free(oldtp->Newick);
            oldtp->Newick = NULL;
        }
    } else {
        return ERR_BAD_FREE;
    }
    
    return ERR_NOERR;
}

int tl_resize_treelist(TLtreelist *trlist)
{
    int ret = ERR_NOERR;
    
    if (trlist->autoinc) {
        int newcapac = trlist->maxtrees + trlist->autoinc;
        TLtopol** templist;
        
        templist = (TLtopol**)realloc(trlist->trees,
                                      newcapac * sizeof(TLtopol*));
        
        if (templist) {
            trlist->trees = templist;
            trlist->maxtrees = newcapac;
        }
        else {
            ret = ERR_BAD_MALLOC;
        }
    }
    else {
        return ERR_BAD_PARAM;
    }
    
    return ret;
}


int tl_attempt_topol_push(TLtopol *topol, TLtreelist* trlist)
{
    if (trlist->ntrees == trlist->maxtrees) {
        if (!trlist->autoinc) {
            return ERR_EXC_CAPACITY;
        }
        
        if (tl_resize_treelist(trlist)) {
            return ERR_BAD_MALLOC;
        }
    }
    

    
    return ERR_NOERR;
}


int tl_push_topol(TLtopol* new, TLtreelist* trlist)
{
    
    if (trlist->ntrees == trlist->maxtrees) {
        if (trlist->autoinc) {
            trlist->maxtrees += trlist->autoinc;
        }
        else {
            return ERR_EXC_CAPACITY;
        }
    }
    
    if (trlist->last) {
        trlist->last->next = new;
    }
    else {
        trlist->first = new;
    }
    
    trlist->last = new;
    
    ++trlist->ntrees;
    
    /*TLtopol* p;
    
    p = trlist->first;
    
    trlist->first   = new;
    new->next       = p;
    if (p) {
        p->back     = new;
    }
    new->back       = NULL;*/
    
    return ERR_NOERR;
}


TLtopol* tl_pop_topol(TLtreelist* trlist)
{
    TLtopol* ret = trlist->last;
    trlist->last = ret->back;
    trlist->last->next = NULL;
    ret->next = NULL;
    ret->back = NULL;
    
    return ret;
}


TLtopol* tl_get_topol(int index, TLtreelist* trlist)
{
    TLtopol *ret = NULL;
    
    ret = trlist->first;
    while (index && ret) {
        ret = ret->next;
        --index;
    }
    
    return ret;
}


int tl_traverse_tree(TLnode *n, int* index, int* postorder)
{
    if (n->tip) {
        dbg_printf("%i", n->tip);
        postorder[*index] = n->index;
        ++(*index);
        return n->index;
    }
    
    dbg_printf("(");
    
    if (n->right) {
        tl_traverse_tree(n->left, index, postorder);
        dbg_printf(",");
        tl_traverse_tree(n->right, index, postorder);
    } else {
        TLnode* p = n->left;
        while (p) {
            tl_traverse_tree(p, index, postorder);
#ifdef DEBUG
            if (p->sib) {
                dbg_printf(",");
            }
#endif
            p = p->sib;
        }
    }
    
    dbg_printf(")");
    postorder[*index] = n->index;
    ++(*index);
    return n->index;
}

int tl_generate_traversal(TLhandlp handle)
{
    return ERR_NOERR;
}


int tl_insert_desc_on_base(TLnode* src, TLnode* tgt)
{
    // TODO: Assert !src->anc
    TLnode** p;
    
    if (!tgt->left) {
        p = &tgt->left;
        *p = src;
    }
    else if (!tgt->right && !tgt->left->sib){
       p = &tgt->right;
       *p = src;
    }
    else {
        TLnode *q = NULL;
        if (tgt->right) {
            assert(!tgt->left->sib);
            q = tgt->right;
            tgt->right = NULL;
            tgt->left->sib = q;
            q->sib = NULL;
            q = NULL;
            tgt->ispolynode = true;
        }
        q = tgt->left->sib;
        tgt->left->sib = src;
        src->sib = q;
    }
    
    src->anc = tgt;
    
    return 0;
}

static inline
void tl_disconnect_all(TLtree* t)
{
    int i = 0;
    int max = tl_upper_root(t).index;
    
    for (i = 0; i < max; ++i) {
        t->trnodes[i].left  = NULL;
        t->trnodes[i].right = NULL;
        t->trnodes[i].sib   = NULL;
        t->trnodes[i].anc   = NULL;
        
        if (i >= t->ntaxa) {
            tl_push_node(&t->trnodes[i], &t->nodestk);
        }
    }
    
    if (t->isrooted) {
        tl_extract_internal_node(t->start);
    }
}

int tl_restore_topol(TLtree* t, TLtopol* topol)
{
    int i = 0;
    
    int max = topol->nnodes;
    
    // Disconnect all the nodal pointers
    /*for (i = 0; i < max; ++i) {
        t->trnodes[i].left  = NULL;
        t->trnodes[i].right = NULL;
        t->trnodes[i].sib   = NULL;
        t->trnodes[i].anc   = NULL;
    }*/
    tl_disconnect_all(t);
    
    for (i = 0; i < max; ++i) {
        
        if (topol->edgeIDs[i] == 0) {
            dbg_printf("Break\n");
        }
        
        tl_insert_desc_on_base(&t->trnodes[i],
                               &t->trnodes[topol->edgeIDs[i]]);
    }
    
    t->start = &tl_upper_root(t);
    
    // TODO: Decide if this is really necessary
    return 1;
}



void tl_reroot_binnode(TLnode* n)
{
    TLnode* p   = NULL;
    TLnode* q   = NULL;
    TLnode** r  = NULL;
    
    if (!n->left->inpath && !n->right->inpath) {
        return;
    }
    
    if (n->left->inpath) {
        p = n->left;
        r = &n->left;
    }
    else /*if (n->right->inpath)*/ {
        p = n->right;
        r = &n->right;
    }
    
    q = n->anc;
    
    *r = q;
    n->anc = p;
   
}

void tl_reroot_polynode(TLnode* n)
{
    if (!n->ispolynode) {
        return;
    }
    
    TLnode** p  = NULL;
    TLnode*  q  = NULL;
    
    *p = n->left;
    
    do {
        if ((*p)->inpath) {
            break;
        }
        q = *p;
        *p = (*p)->sib;
    } while (*p);
    
    if (!*p) {
        return;
    }
    
    // TODO: Finish this function
    
}


void tl_mark_downpass(TLnode* n, int index, bool* found)
{
    if (*found) {
        return;
    }
    
    if (n->index == index) {
        dbg_printf("Found the node: %i\n", index);
        *found      = true;
        n->inpath   = true;
        return;
    }
    
    if (n->tip) {
        return;
    }
    
    if (n->right) {
        tl_mark_downpass(n->left, index, found);
        tl_mark_downpass(n->right, index, found);
    } else {
        TLnode* p = n->left;
        while (p) {
            tl_mark_downpass(p, index, found);
            p = p->sib;
        }
    }
    
    if (*found) {
        n->inpath = true;
    }
}

int tl_make_node_available(TLnode *n)
{
    // This is a somewhat dangerous function to use as it will cut any node it
    // is called on from the tree.
    // TODO: Could put a flag in the node that is set/unset by functions that
    // are allowed to call this one (i.e. functions that push/pull available
    // nodes from the stack.
    
    if (!n) {
        return -1;
    }
    
    n->left     = NULL;
    n->right    = NULL;
    n->anc      = NULL;
    n->sib      = NULL;
    return 0;
}

int tl_reroot_subtree(TLnode *subtr, int* postorder)
{
    int ret = 0;
    
    
    
    return ret;
}

int tl_root_tree(TLtree* t, int tgtroot, bool polybase)
{
    int ret = 0;
    
    // Check existing root
    if (t->isrooted) {
        
        // Check that it's not the same root requested
        if (t->start->left->index == tgtroot ||
            t->start->right->index == tgtroot)
        {
            return 1;
        }
        
    }
    
    // Mark path to target root (tgtroot) base
    bool found = false;
    tl_mark_downpass(t->start, tgtroot, &found);
    found = false;
    
    int i = 0;
    //int numnodes = tl_num_nodes(t);
    
    // If rooted, extract the old root
    if (t->isrooted) {
        TLnode* bleft   = t->start->left;
        TLnode* bright  = t->start->right;
        bleft->anc  = bright;
        bright->anc = bleft;
    }
    
    t->start = &tl_upper_root(t);
    
    for (i = t->ntaxa; i < tl_upper_root(t).index; ++i) {
        if (t->trnodes[i].inpath) {
            // TODO: Check for binnode
            tl_reroot_binnode(&t->trnodes[i]);
        }
    }
    
    for (i = 0; i < tl_upper_root(t).index; ++i) {
        t->trnodes[i].inpath = false;
    }
    
    t->start->left          = &t->trnodes[tgtroot];
    t->start->right         = t->trnodes[tgtroot].anc;
    t->start->left->anc     = t->start;
    t->start->right->anc    = t->start;
    t->start->inpath        = false;

    // TODO: If polybase, collapse the root node and set ispolynode = true
    // TODO: The tree needs to be traverse before returning.
    
    return ret;
}

void tl_mark_nodes(TLnode* n, int* index, TLtopol* topol)
{
    
    if (n->downpmark) {
        return;
    }
    
    if (n->anc == n || n->tip) {
        return;
    }
    
    n->downpmark = *index;
    
   
    ++(*index);
    
    tl_mark_nodes(n->anc, index, topol);
    
    topol->edgeIDs[n->downpmark] = n->anc->downpmark;
}

TLnode* tl_find_anytip(TLnode* n)
{
    if (n->tip) {
        return n;
    }
    
    TLnode* ret = NULL;
    
    if ((ret = tl_find_anytip(n->left))) {
        return ret;
    }
    
    if ((ret = tl_find_anytip(n->right))) {
        return ret;
    }
    
    return ret;
}

void tl_extract_internal_node(TLnode* n)
{
    TLnode* p = n->left;
    TLnode* q = n->right;
    n->left = NULL;
    n->right = NULL;
    p->anc = q;
    q->anc = p;
}

/* Not entirely safe function that will quickly insert a root at the current
 * start node on the tree. This is useful for tree length calculations and 
 * optimisations in trees that are unrooted during the search but need to be 
 * rooted for length calculations. */
void tl_quick_root(TLtree* t)
{
    if (t->isrooted) {
        return;
    }
    
    // If this fails, the function has been called in the wrong place
    assert(t->start->anc->anc == t->start);
   
    // Get the node reserved for the root
    TLnode* rt = &tl_upper_root(t);
    
    // Point its left and right to the current start
    rt->left    = t->start->anc;
    rt->right   = t->start;
    
    // Point the ancestors of left and right back to the root
    rt->left->anc   = rt;
    rt->right->anc  = rt;
    
    t->start = rt;
    
    // Set the rooted flag
    t->isrooted = 1;
}


int tl_unroot_tree(TLtree* t)
{
    if (t->isrooted == 0) {
        return 1;
    }
    
    TLnode *tip = NULL;
	// If there's a tip neighboring the root, use it.
	if (t->start->left->tip > 0) {
		tip = t->start->left;
	}
	else if (t->start->right->tip > 0) {
		tip = t->start->right;
	}
	else {
    // Find an internal tip
		tip = tl_find_anytip(t->start);
	}

    assert(tip);
   
    int index = tip->index;
    
    // Mark downpass to that tip
    bool found = false;
    tl_mark_downpass(t->start, index, &found);
    found = false;
    
    // Reroot at that tip
    tl_root_tree(t, index, false);
    
    // Remove the root
    //tl_extract_internal_node(t->start);
    
    // Set start to the internal node neighboring the root tip 
    //t->start->anc = tip;
    //tip->left = t->start;
    //tip->right = NULL;
//    t->start = tip->anc;

	
    
    t->isrooted = 0;
    
    return 0;
}


void tl_mark_for_comparison(TLtree* t, TLtopol* topol)
{
    int i = 0;
    int ntax = t->ntaxa;
    int index = ntax;
   
    
    for (i = 0; i < ntax; ++i) {
        tl_mark_nodes(t->trnodes[i].anc, &index, topol);
        topol->edgeIDs[i] = t->trnodes[i].anc->downpmark;
    }
    
    for (i = 0; i < t->nnodes; ++i) {
        t->trnodes[i].downpmark = 0;
    }
    
}


int tl_compare_topols(TLtopol* tp1, TLtopol* tp2)
{
    int ret = 0;
    
    ret = memcmp(tp1->edgeIDs, tp2->edgeIDs, tp1->nnodes * sizeof(int));
    
    return ret;
}


int tl_push_node(const TLnode* n, TLndstack *ndstk)
{
	int ret = 0;

	if (ndstk->nnodes < ndstk->maxnodes) {
		if (!ndstk->stack[ndstk->nnodes]) {
			ndstk->stack[ndstk->nnodes] = (TLnode*)n;
			ndstk->head = ndstk->stack[ndstk->nnodes];
			++ndstk->nnodes;
			ret = 0;
		}
		else {
			ret = -2;
		}
	}
	else {
		ret = -1;
	}

	return ret;
}


TLnode* tl_pull_node(TLndstack *ndstk)
{
	TLnode* ret = NULL;

	if (ndstk->nnodes) {

		ret = ndstk->head;
		--ndstk->nnodes;

		if (ndstk->nnodes) {
			--ndstk->head;
		}
		else {
			ndstk->head = NULL;
		}

		if (ndstk->nnodes < 0) {
			ret = NULL; // A conservative error: somehow, numnodes
		}				// went negative, so return nothing.		
	}
	
	return ret;
}

TLnode* tl_find_sib(TLnode* n)
{
    if (n->anc == NULL) {
        return NULL;
    }
    
    if (n->anc->left == n) {
        return n->anc->right;
    }
    else {
        return n->anc->left;
    }
}

TLnode* tl_remove_branch(TLnode* src, TLtree* t)
{
    TLnode* tgtop = NULL;
    TLnode* tgbottom = NULL;
    
    tgtop = tl_find_sib(src);
    tgbottom = src->anc->anc;
    
    assert(tgtop->anc->anc == src->anc->anc);
    TLnode** bback = NULL;
    if (tgbottom->left == src->anc) {
        bback = &tgbottom->left;
    } else {
        bback = &tgbottom->right;
    }
    
    *bback = tgtop;
    tgtop->anc = tgbottom;
    
    src->anc->anc = NULL;
    
    if (src->anc->left == src) {
        src->anc->right = NULL;
    }
    else {
        src->anc->left = NULL;
    }
    
    
    return tgtop;
}

TLnode** tl_get_anc_pointer(TLnode* n)
{
    TLnode** ret = NULL;
    
    if (n->anc->ispolynode) {
        // Probably won't work
        TLnode* p = n->left;
        while (p != n) {
            p = p->sib;
        }
        *ret = p;
    } else {
        if (n->anc->left == n) {
            *ret = n->anc->left;
        } else {
            *ret = n->anc->right;
            assert(n->anc->right == n);
        }
    }
    
    return ret;
}


int tl_insert_branch(TLnode* src, int tgt, TLtree* t)
{
    TLnode*  n  = NULL;
    TLnode*  p  = NULL;
    TLnode** q  = NULL;
    
    n = &t->trnodes[tgt];
    p = n->anc;
    
    if (p->left == n)
    {
        q = &p->left;
    }
    else {
        q = &p->right;
    }
    
    *q = src->anc;
    src->anc->anc = p;
    
    n->anc = src->anc;
    
    if (src->anc->left == src)
    {
        src->anc->right = n;
    }
    else {
        src->anc->left = n;
    }
    
    return 0;
}


// TODO: Expose
void tl_create_starting_fork(int left, int right, int anc, TLtree* t)
{
    tl_disconnect_all(t);
    tl_unroot_tree(t);
    
    TLnode* n = tl_pull_node(&t->nodestk);
    
    n->left = &t->trnodes[left];
    n->left->anc = n;
    n->right = &t->trnodes[right];
    n->right->anc = n;
    n->anc = &t->trnodes[anc];
    n->anc->anc = n;
    
    t->start = n;
    t->isrooted = 0;
}

