#include "tldefs.h"
#include "treelib.h"
#include "tltree.h"
#include "tlnewick.h"
#include "tlutils.h"
#include "tlerror.h"

//#include <stdio.h>

TLP tl_TLhandle2TLP(TLhandlp TLhandle)
{
    return (TLP)TLhandle;
}


TLhandlp tl_TLP2TLhandle(TLP tlp)
{
    return (TLhandlp)tlp;
}


TLtree *tl_createtree(const int ntaxa)
{
    TLtree *newtr = NULL;
    
    if (ntaxa) {
        newtr = (TLtree*)tl_alloc(sizeof(TLtree), 0);
        
        newtr->ntaxa    = ntaxa;
        newtr->trnodes  = (TLnode*)tl_alloc(tl_calc_nnodes(ntaxa)
                                            * sizeof(TLnode),  0);
    }

	return newtr;	
}

//int tl_destroytree(TLtree *t)
//{
//    if (t) {
//        free(t->trnodes);
//        free(t);
//        return 0;
//    }
//    else {
//        return -1;
//    }
//}


int append_topology(TLtopol *newtopo, TLP tlp)
{
    TLhandlp trl = tl_TLP2TLhandle(tlp);
	int ret = 0;
	
    if (trl->treelist.ntrees < trl->treelist.maxtrees) {
		
        trl->treelist.last->next    = newtopo;
		trl->treelist.last          = trl->treelist.last->next;
		++trl->treelist.ntrees;
	
    }
	else {
		ret = -1;
	}
    
    return ret;
}


TLhandlp tl_newTLstruct(void)
{
    TLhandlp tlret = tl_alloc(sizeof(TLhandl), 0);
    
    // Setup the taxlabel list
    tlret->taxlabels.nelems     = 0;
    tlret->taxlabels.maxelems   = 0;
    
    // Setup the treelist
    tlret->treelist.ntrees      = 0;
    tlret->treelist.maxtrees    = DEFAULT_MAXTREES;
    tlret->treelist.autoinc     = DEFAULT_AUTOINC;
    tlret->treelist.trees       = (TLtopol**)tl_alloc(DEFAULT_MAXTREES *
                                                      sizeof(TLtopol*), 0);
    return tlret;
}


TLP tl_new_TL(void)
{
    TLP ret = tl_TLhandle2TLP(tl_newTLstruct());
	return ret;
}


int tl_delete_TL(TLP handle)
{
    if (handle) {
        TLhandlp oldtlp = tl_TLP2TLhandle(handle);
        
        // Destroy the strling list of taxlables
        if (oldtlp->taxlabels.labels) {
            int i = 0;
            for (i = 0; i < oldtlp->taxlabels.nelems; ++i) {
                if (oldtlp->taxlabels.labels[i]) {
                    if (oldtlp->taxlabels.labels[i]->label) {
                        free(oldtlp->taxlabels.labels[i]->label);
                        oldtlp->taxlabels.labels[i]->label = NULL;
                    }
                    free(oldtlp->taxlabels.labels[i]);
                    oldtlp->taxlabels.labels[i] = NULL;
                }
            }
        }
        // TODO: Destroy the tree
        tl_destroy_TLtree(oldtlp->tree);
        
        // Destroy the treelist
        
        
        oldtlp->dimensions.ntaxa = 0;
        free(oldtlp);
        
        return 0;
    }
    return -1;
}


int tl_set_current_tree(int index, TLP trl)
{
    if (!trl) {
        return ERR_NULL_PTR;
    }
    TLhandlp handl = (TLhandlp)trl;
    if (!handl->tree) {
        handl->tree = tl_new_TLtree(tl_get_numtaxa(trl));
    }
    
    TLtopol* topol = tl_get_topol(index, &handl->treelist);
    if (!topol) {
        return ERR_INDEX_OUT_OF_RANGE;
    }
    
    tl_restore_topol(handl->tree, topol);
    
    // TODO: Do a real root
    
    int j = 0;
    dbg_printf("Traversing tree:\n");
    tl_traverse_tree(handl->tree->start, &j, handl->tree->postorder);
    dbg_printf("\n");
    
    // Record topology info if needed.
    // TODO: Only do this if necessary
    tl_mark_for_comparison(handl->tree, topol);
    
    if (!topol->rooted) {
        // Extract the root
    }
    else {
        handl->tree->isrooted = true;
    }
    
    return ERR_NOERR;
}


int tl_do_contree(TLconsens_t contype, int cutoff, TLP trl)
{
    return ERR_NOT_IMPLEMENTED;
}


int tl_attach_Newick(const char *Newick, TLP trl)
{
    if (!trl) {
        return ERR_NULL_PTR;
    }
    
    TL_ERR_T err = ERR_NOERR;
    
    TLhandlp handle = (TLhandlp)trl;
    err = tl_read_Newick(Newick, handle);
    
    return err;
}

char* tl_get_Newick(int trnum, TLP trl)
{
    char *nwk=NULL;
    return nwk;
}

char* tl_get_contreeNewick(TLconsens_t contype, TLP trl)
{
    char *nwk=NULL;
    return nwk;
}

int tl_store_topology(const TLtree *t, TLP trl)
{
    return 1;
}

int tl_rand_topol(TLP trl)
{
    return 1;
}

int alltopologies(TLP trl)
{
    return 1;
}

int tl_same_tree(int tr1, TLP trl1, int tr2, TLP trl2)
{
    TLtopol* tp1 = tl_get_topol(tr1, &((TLhandlp)trl1)->treelist);
    TLtopol* tp2 = tl_get_topol(tr2, &((TLhandlp)trl2)->treelist);
    
    return tl_compare_topols(tp1, tp2);
}

int compattree(int tr1, TLP trl1, int tr2, TLP trl2)
{
    return 1;
}

int removeduplicates(TLP trl)
{
    return 1;
}

/*!
 @brief Attaches a new terminal label to the internal list of taxa.
 
 @discussion  This function will only work if there are no trees loaded in
 memory. When a new taxon label is added to the list, the number of taxa in the
 dataset is updated.
 
 @param taxlabel C-style string representing the terminal label.
 @param tlp The TLP data structure
 @return Error if the function fails to attach a taxon label.
 */
int tl_attach_taxon_label(char *taxlabel, TLP tlp)
{
    if (!tlp) {
        return ERR_NULL_PTR;
    }
    
    // This needs to pre-process taxon labels for punctuation and spaces
    
    TLhandlp handl = (TLhandlp)tlp;
        
    if (!handl->treelist.ntrees) { // TODO: Perhaps use more direct check
        if (tl_check_duplicate_label(taxlabel, &handl->taxlabels)) {
            
            // TODO: Check that this doesn't cause a memory leak!
            char *tlcopy = tl_strdup(taxlabel);
            
            if (!tl_push_new_label(tlcopy, &handl->taxlabels)) {
                handl->dimensions.ntaxa++;
                free(tlcopy);
                return 0;
            }
            else {
                free(tlcopy);
                return ERR_BAD_MALLOC;
            }
        }
        else {
            return ERR_EX_DAT_CONFLICT; // Duplicate string
        }
    }
    else {
        return ERR_EX_DAT_CONFLICT; 
    }
    
}

char* tl_get_taxon_label(int index, TLP tlp)
{
    if (tlp) {
        TLhandlp handl = (TLhandlp)tlp;
        if (index < handl->taxlabels.nelems) {
             return handl->taxlabels.labels[index]->label;
        } else {
            return NULL; // TODO: Index out of bounds
        }
        
    } else {
        return NULL; // TODO: Expected valid ptr
    }
}

// TODO: consider whether this really should be exposed
int tl_set_numtaxa(int ntaxa, TLP tlp)
{
    if (tlp) {
        TLhandlp trl = (TLhandlp)tlp;
       
        if (trl->dimensions.ntaxa != 0) {
            trl->dimensions.ntaxa = ntaxa;
            
            // TODO: Reallocate the tree
            
        } else {
            trl->dimensions.ntaxa = ntaxa;
            trl->tree = tl_new_TLtree(trl->dimensions.ntaxa);
        }
        
        return ERR_NOERR;
    }
    
    return ERR_NULL_PTR;
}


int tl_get_numtaxa(TLP tlp)
{
    if (tlp) {
        TLhandlp trl = (TLhandlp)tlp;
        return trl->dimensions.ntaxa;
    }
    
    return ERR_NULL_PTR;
}


int tl_set_maxtrees(int maxtrees, TLP tlp)
{
    
    int ret = ERR_NOERR;
    if (tlp) {
        TLhandlp trl = (TLhandlp)tlp;
        trl->treelist.maxtrees = maxtrees;
    }
    else {
        ret = ERR_NULL_PTR;
    }
    
    return ret;
}


/*!
 Sets a value for automatically increasing the size of the tree buffer. This
 function increases the amount of storage for additional trees that was 
 originally specified by the setmaxtrees function.

 @param increment The amount of additional space to be allocated for storage of
 trees.
 @param tlp An instance of a TLP structure.
 @return 0 upon success, numeric error code if there's an error.
 */
int tl_set_autoinc(int increment, TLP tlp)
{
    TLhandlp trl = (TLhandlp)tlp;
    int ret = ERR_NOERR;
    if (trl) {
        trl->treelist.autoinc = increment;
        ret = ERR_NOERR;
    }
    else {
        ret = ERR_NULL_PTR;
    }
    
    return ret;
}


int tl_get_num_trees(TLP tlp)
{
    if (!tlp) {
        return ERR_NULL_PTR;
    }
    
    TLhandlp trl = (TLhandlp)tlp;
    
    return trl->treelist.ntrees;
}
