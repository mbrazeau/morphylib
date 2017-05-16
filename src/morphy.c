//
//  morphy.c
//  MorPhy2
//
//  Created by mbrazeau on 23/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "morphydefs.h"
#include "mplerror.h"
#include "morphy.h"
#include "statedata.h"
#include "fitch.h"
#include "mpl.h"

void *mpl_alloc(size_t size, int setval)
{
    void *ret = malloc(size);
    if (ret) {
        memset(ret, setval, size);
    }
    return ret;
}


Morphyp mpl_new_Morphy_t(void)
{
    Morphyp new = (Morphyp)calloc(1, sizeof(Morphy_t));
    
    // Set defaults:
    mpl_set_gaphandl(GAP_INAPPLIC, (Morphy)new);
    new->symbols.gap        = DEFAULTGAP;
    new->symbols.missing    = DEFAULTMISSING;
    
    return new;
}


void* mpl_get_from_matrix
(const int      row,
 const int      col,
 const int      ncol,
 const size_t   size,
 const void*    data)
{
    return (void*)(data + (row * ncol * size + (size * col)));
}


int mpl_get_gaphandl(Morphyp handl)
{
    assert(handl);
    return handl->gaphandl;
}


//int     mpl_set_num_charac(const int nchar, Morphy m);
int mpl_set_num_charac(const int nchar, Morphyp m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    m->numcharacters = nchar;
    
    return ERR_NO_ERROR;
}


//int     mpl_set_numtaxa(const int ntax, Morphy m);
int mpl_set_numtaxa(const int ntax, Morphyp m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    m->numtaxa = ntax;
    
    return ERR_NO_ERROR;
}


int mpl_check_data_loaded(Morphyp m)
{
    if (m->char_t_matrix) {
        return 1;
    }
    
    return 0;
}


char mpl_get_gap_symbol(Morphyp handl)
{
    return handl->symbols.gap;
}

//MPLarray* mpl_new_array(size_t elemsize)
//{
//    MPLarray *new = calloc(1, sizeof(MPLarray));
//    if (!new) {
//        return NULL;
//    }
//    
//    new->data = (void**)calloc(1, elemsize);
//    if (!new->data) {
//        free(new);
//        return NULL;
//    }
//    new->elemsize = elemsize;
//    new->maxelems = 1;
//    new->nelems = 0;
//    
//    return new;
//}
//
//
//void mpl_destroy_array(MPLarray* arr)
//{
//    if (arr->data) {
//        free(arr->data);
//    }
//    if (arr) {
//        free(arr);
//    }
//}
//
//int mpl_array_push(void* data, MPLarray* arr)
//{
//    if (arr->nelems <= arr->maxelems) {
//        arr->data[arr->nelems] = data;
//        ++arr->nelems;
//        return ERR_NO_ERROR;
//    }
//    
//    void** newdat = realloc(arr->data, (arr->maxelems + 1) * arr->elemsize);
//    if (!newdat) {
//        return ERR_BAD_MALLOC;
//    }
//    
//    ++arr->nelems;
//    ++arr->maxelems;
//    arr->data[arr->nelems] = data;
//    
//    return ERR_NO_ERROR;
//}

void mpl_assign_fitch_fxns(MPLpartition* part)
{
    assert(part);
    
    if (part->isNAtype) {
        part->inappdownfxn  = mpl_NA_fitch_second_downpass;
        part->inappupfxn    = mpl_NA_fitch_second_uppass;
        part->prelimfxn     = mpl_NA_fitch_first_downpass;//mpl_NA_fitch_second_downpass;
        part->finalfxn      = mpl_NA_fitch_first_uppass;
    }
    else {
        part->prelimfxn     = mpl_fitch_downpass;
        part->finalfxn      = mpl_fitch_uppass;
        part->inappdownfxn  = NULL; // Not necessary, but safe & explicit
        part->inappupfxn    = NULL;
    }
}


int mpl_fetch_parsim_fxn_setter
(void(**pars_assign)(MPLpartition*), MPLchtype chtype)
{
    int err = ERR_NO_ERROR;
    
    switch (chtype) {
        case FITCH_T:
            if (pars_assign) {
                *pars_assign = &mpl_assign_fitch_fxns;
            }
            break;
            
            // TODO: Implement other functions here
        default:
            err = ERR_CASE_NOT_IMPL;
            break;
    }
    
    return err;
}


int mpl_assign_partition_fxns(MPLpartition* part)
{
    assert(part);
    int err = ERR_NO_ERROR;
//    MPLchtype chtype = part->chtype;
//    assert(chtype);
    
    void (*pars_assign)(MPLpartition*);
    
    err = mpl_fetch_parsim_fxn_setter(&pars_assign, part->chtype);
    
    pars_assign(part);
    
    return err;
}


int mpl_extend_intarray(int** array, size_t size)
{
    int* temp = (int*)realloc(*array, size);
    if (!temp) {
        return ERR_BAD_MALLOC;
    }
    
    *array = temp;
    
    return ERR_NO_ERROR;
}


int mpl_part_push_index(int newint, MPLpartition* part)
{
    int err = ERR_NO_ERROR;
    
    if (part->ncharsinpart < part->maxnchars) {
        part->charindices[part->ncharsinpart] = newint;
        ++part->ncharsinpart;
    }
    else {
        err = mpl_extend_intarray(&part->charindices,
                                 (part->maxnchars + 1) * sizeof(int));
        if (!err) {
            part->charindices[part->ncharsinpart] = newint;
            ++part->ncharsinpart;
            ++part->maxnchars;
        }
    }
    
    return err;
}


int mpl_part_remove_index(int index, MPLpartition* part)
{
    if (!part->ncharsinpart) {
        return 1;
    }
    
    --part->ncharsinpart;
    assert(part->ncharsinpart >= 0);
    
    int i = 0;
    for (i = 0; i < part->ncharsinpart; ++i) {
        part->charindices[i] = part->charindices[i + 1];
    }
    part->charindices[i] = MPLCHARMAX; // Gives some clue if an error occurs
    
    return 0;
}


int mpl_delete_partition(MPLpartition* part)
{
    int err = ERR_UNEXP_NULLPTR;
    
    if (part) {
        if (part->charindices) {
            free(part->charindices);
            part->charindices   = NULL;
            part->maxnchars     = 0;
            part->ncharsinpart  = 0;
        }
        free(part);
        err = ERR_NO_ERROR;
    }
    
    return err;
}


MPLpartition* mpl_new_partition(const MPLchtype chtype, const bool hasNA)
{
    assert(chtype);
    
    MPLpartition *new = (MPLpartition*)calloc(1, sizeof(MPLpartition));
    
    if (!new) {
        return NULL;
    }
    
    new->chtype     = chtype;
    new->isNAtype   = hasNA;

    new->charindices = (int*)calloc(1, sizeof(int));
    if (!new->charindices) {
        free(new);
        return NULL;
    }
    new->maxnchars      = 1;
    new->ncharsinpart   = 0;
    
    mpl_assign_partition_fxns(new);
    
    return new;
}


int mpl_count_gaps_in_columns(Morphyp handl)
{
    int i = 0;
    int j = 0;
    char gap            = mpl_get_gap_symbol(handl);
    int numchar         = mpl_get_num_charac((Morphy)handl);
    int numtax          = mpl_get_numtaxa((Morphy)handl);
    MPLmatrix* matrix   = mpl_get_mpl_matrix(handl);
    MPLcharinfo* chinfo = handl->charinfo;
    int numna = 0;
    
    for (i = 0; i < numchar; ++i) {
        chinfo[i].ninapplics = 0;
        for (j = 0; j < numtax; ++j) {
            
            MPLcell* cell = &matrix->cells[j * numchar + i];
            
            if (strchr(cell->asstr, gap)) {
                ++chinfo[i].ninapplics;
            }
            
            // Once the number of NAs exceeds 2, then we can be satisfied that
            // there are sufficient NAs to apply NA functions, otherwise, just
            // treat it as a gap
            if (chinfo[i].ninapplics > NACUTOFF) {
                ++numna;
                break;
            }
        }
    }
    
    return numna;
}


int mpl_compare_partition_with_char_info
(const MPLcharinfo *chinfo, const MPLpartition* part, const gap_t gaphandl)
{
    int ret = 0;
    
    if (chinfo->chtype != part->chtype) {
        ++ret;
    }
    
    if (gaphandl == GAP_INAPPLIC) {
        if (chinfo->ninapplics <= NACUTOFF) {
            if (part->isNAtype) {
                ++ret;
            }
        }
        else {
            if (!part->isNAtype) {
                ++ret;
            }
        }
    }
    
    return ret;
}


/*!
 @brief Searches the partition list for a partition matching the supplied info
 @discussion Traverses a linked list of partitions, looking for a partition 
 matching the supplied information. If this function returns NULL, then the
 supplied info does not match a character in the list. A new partition will
 need to be created.
 @param chinfo MPLchtype providing data on a character in the matrix.
 @param part A data partition; should be the first partition in the list.
 @return A pointer to the partition corresponding to the supplied character
 information.
 */
MPLpartition* mpl_search_partitions
(MPLcharinfo *chinfo, MPLpartition* part, gap_t gaphandl)
{
    assert(chinfo);
    MPLpartition* p = part;
    
    while (p) {
        if (!mpl_compare_partition_with_char_info(chinfo, p, gaphandl)) {
            return p;
        }
        p = p->next;
    }
    
    return p;
}


int mpl_compare_partitions(const void* ptr1, const void* ptr2)
{
    MPLpartition* part1 = *(MPLpartition**)ptr1;
    MPLpartition* part2 = *(MPLpartition**)ptr2;
    
    int ret;
    MPLchtype cdiff = NONE_T;
    
    cdiff = part1->chtype - part2->chtype;
    ret = (int)cdiff;
    
    if (!cdiff) {
        if (part2->isNAtype) {
            ret = 1;
        }
        else {
            ret = 0;
        }
    }
    
    return ret;
}


int mpl_put_partitions_in_handle(MPLpartition* first, Morphyp handl)
{
    handl->partitions = (MPLpartition**)calloc(handl->numparts,
                                                sizeof(MPLpartition*));
    if (!handl->partitions) {
        return ERR_BAD_MALLOC;
    }
    
    int i = 0;
    MPLpartition* p = first;
    while (p) {
        handl->partitions[i] = p;
        ++i;
        p = p->next;
    }
    assert(i == handl->numparts);
    
    // Sort the partitions.
    qsort(handl->partitions, handl->numparts, sizeof(MPLpartition*), mpl_compare_partitions);
    handl->partstack = first;
    
    return ERR_NO_ERROR;
}


int mpl_setup_partitions(Morphyp handl)
{
    assert(handl);
    
    int err = ERR_NO_ERROR;
    
    int i = 0;
    int nchar = mpl_get_num_charac((Morphyp)handl);
    
    MPLcharinfo* chinfo = NULL;
    MPLpartition* first = NULL;
    MPLpartition* last  = NULL;
    MPLpartition* p     = NULL;
    int numparts        = 0;
    
    for (i = 0; i < nchar; ++i) {
        // Examine the character info for each character in the matrix
        chinfo = &handl->charinfo[i];
        
        if (chinfo->included) {
            p = mpl_search_partitions(chinfo, first, mpl_get_gaphandl(handl));
            
            if (p) {
                mpl_part_push_index(i, p);
            }
            else {
                bool hasNA = false;
                if (handl->gaphandl == GAP_INAPPLIC) {
                    if (chinfo->ninapplics > NACUTOFF) {
                        hasNA = true;
                    }
                }
                p = mpl_new_partition(chinfo->chtype, hasNA);
                //            last->next =
                mpl_part_push_index(i, p);
                if (!first) {
                    first = p;
                    last = p;
                }
                else {
                    last->next = p;
                    last = p;
                }
                
                ++numparts;
            }
        }
    }
    
    handl->numparts = numparts;
    err = mpl_put_partitions_in_handle(first, handl);
    
    return err;
}


int mpl_get_numparts(Morphyp handl)
{
    return handl->numparts;
}


MPLndsets* mpl_alloc_stateset(int numchars)
{
    MPLndsets* new = (MPLndsets*)calloc(1, sizeof(MPLndsets));
    if (!new) {
        return NULL;
    }
    
    new->NApass1 = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->NApass1) {
        mpl_free_stateset(new);
    }
    
    new->NApass2 = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->NApass2) {
        mpl_free_stateset(new);
    }
    
    new->prelimset = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->prelimset) {
        mpl_free_stateset(new);
    }
    
    new->finalset = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->finalset) {
        mpl_free_stateset(new);
    }
    
    new->subtree_actives = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->subtree_actives) {
        mpl_free_stateset(new);
    }
    
    new->subtree_NApass1 = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->subtree_NApass1) {
        mpl_free_stateset(new);
    }
    
    new->subtree_NApass2 = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->subtree_NApass2) {
        mpl_free_stateset(new);
    }
    
    new->subtree_prelimset = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->subtree_prelimset) {
        mpl_free_stateset(new);
    }
    
    new->subtree_finalset = (MPLstate*)calloc(1, numchars * sizeof(MPLstate));
    if (!new->subtree_finalset) {
        mpl_free_stateset(new);
    }
    
    return new;
}


void mpl_free_stateset(MPLndsets* statesets)
{
    if (statesets->NApass1) {
        free(statesets->NApass1);
        statesets->NApass1 = NULL;
    }
    if (statesets->NApass2) {
        free(statesets->NApass2);
        statesets->NApass2 = NULL;
    }
    if (statesets->prelimset) {
        free(statesets->prelimset);
        statesets->prelimset = NULL;
    }
    if (statesets->finalset) {
        free(statesets->finalset);
        statesets->finalset = NULL;
    }
    if (statesets->subtree_actives) {
        free(statesets->subtree_actives);
        statesets->subtree_actives = NULL;
    }
    if (statesets->subtree_NApass1) {
        free(statesets->subtree_NApass1);
        statesets->subtree_NApass1 = NULL;
    }
    if (statesets->subtree_NApass2) {
        free(statesets->subtree_NApass2);
        statesets->subtree_NApass2 = NULL;
    }
    if (statesets->subtree_prelimset) {
        free(statesets->subtree_prelimset);
        statesets->subtree_prelimset = NULL;
    }
    if (statesets->subtree_finalset) {
        free(statesets->subtree_finalset);
        statesets->subtree_finalset = NULL;
    }
    
    free(statesets);
}


int mpl_setup_statesets(Morphyp handl)
{
    MPL_ERR_T err = ERR_NO_ERROR;
    
    // TODO: Implement total numnodes getter
    int numnodes = handl->numnodes;
    handl->statesets = (MPLndsets**)calloc(numnodes,
                                              sizeof(MPLndsets*));
    if (!handl->statesets) {
        return ERR_BAD_MALLOC;
    }
    
    int i = 0;
    int nchars = mpl_get_num_charac((Morphyp)handl);
    
    for (i = 0; i < numnodes; ++i) {
        if (!(handl->statesets[i] = mpl_alloc_stateset(nchars))) {
            err = ERR_BAD_MALLOC;
            mpl_destroy_statesets(handl);
            break;
        }
    }
    
    return ERR_NO_DATA; // TODO: Replace
}


int mpl_destroy_statesets(Morphyp handl)
{
    int i = 0;
    // TODO: Implement total numnodes getter
    int numnodes = handl->numnodes;
    
    
    
    if (handl->statesets) {
        
        for (i = 0; i < numnodes; ++i) {
            mpl_free_stateset(handl->statesets[i]);
        }
        
        free(handl->statesets);
        handl->statesets = NULL;
    }
    
    return ERR_NO_ERROR; // TODO: Replace
}


int mpl_copy_data_into_tips(Morphyp handl)
{
    int i = 0;
    int j = 0;
    int ntax = mpl_get_numtaxa((Morphy)handl);
    int nchar = mpl_get_num_charac((Morphy)handl);
    MPLndsets** nsets = handl->statesets;
    
    for (i = 0; i < ntax; ++i) {
        for (j = 0; j < nchar; ++j) {
            nsets[i]->prelimset[j] =
            handl->inmatrix.cells[i * nchar + j].asint;
            nsets[i]->NApass1[j] = nsets[i]->prelimset[j];
        }
    }
    
    return ERR_NO_ERROR;
}
