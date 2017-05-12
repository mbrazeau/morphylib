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

