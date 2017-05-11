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
    return (void*)(data + row * col * size + (size * col - 1));
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

int mpl_down_recon
(const int nodeID, const int lChild, const int rChild, Morphy m)
{
    return 1;
}
