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
    
    part->prelimfxn     = mpl_fitch_downpass;
    part->finalfxn      = mpl_fitch_uppass;
    part->inappdownfxn  = NULL; // Not necessary, but safe & explicit
    part->inappupfxn    = NULL;
}

void mpl_assign_fitch_NA_fxns(MPLpartition* part)
{
    assert(part);

    part->inappdownfxn  = mpl_NA_fitch_first_downpass;
    part->inappupfxn    = mpl_NA_fitch_first_uppass;
    part->prelimfxn     = mpl_NA_fitch_second_downpass;
    part->finalfxn      = mpl_NA_fitch_second_uppass;
    
}


int mpl_assign_partition_fxns(MPLpartition* part)
{
    assert(part);
    int err = ERR_NO_ERROR;
    MPLchtype chtype = part->chtype;
    assert(chtype);
    
    switch (chtype) {
        case FITCH_T:
            if (part->isNAtype) {
                mpl_assign_fitch_NA_fxns(part);
            }
            else {
                mpl_assign_fitch_fxns(part);
            }
            break;
            
        // TODO: Implement other functions here
        default:
            err = ERR_CASE_NOT_IMPL;
            break;
    }
    
    
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

int mpl_setup_partitions(Morphyp handle)
{
    assert(handle);
    
    int err = ERR_NO_ERROR;
    
    int i = 0;
    int nchar = mpl_get_num_charac((Morphyp)handle);
    
    MPLcharinfo* chinfo = NULL;
    MPLpartition* first = NULL;
    MPLpartition* last  = NULL;
    MPLpartition* p     = NULL;
    int numparts        = 0;
    
    for (i = 0; i < nchar; ++i) {
        // Examine the character info for each character in the matrix
        chinfo = &handle->charinfo[i];
        
        if (chinfo->included) {
            p = mpl_search_partitions(chinfo, first, mpl_get_gaphandl(handle));
            
            if (p) {
                mpl_part_push_index(i, p);
            }
            else {
                bool hasNA = false;
                if (handle->gaphandl == GAP_INAPPLIC) {
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
    
    handle->numparts = numparts;
    
    return err;
}

int mpl_get_numparts(Morphyp handl)
{
    return handl->numparts;
}
