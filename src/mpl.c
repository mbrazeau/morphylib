//
//  mpl.c
//  MorPhy2
//
//  Created by mbrazeau on 04/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "morphydefs.h"
#include "mplerror.h"
#include "mpl.h"
#include "morphy.h"
#include "statedata.h"

Morphy mpl_new_Morphy(void)
{
    Morphyp new     = mpl_new_Morphy_t();
    
//    if (new) {
//        new->symboldict = mpl_alloc_symbolset();
//    }
    
    return (Morphy)new;
}



int mpl_delete_Morphy(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    Morphyp m1 = (Morphyp)m;
    
    // TODO: All Morphy destructors
    mpl_delete_rawdata(m);
    mpl_destroy_symbolset(m1);
    mpl_delete_mpl_matrix(m1->inmatrix);
    free(m1);
    
    return ERR_NO_ERROR;
}


int mpl_init_Morphy(const int ntax, const int nchar, Morphy m)
{
    if (!ntax || !nchar) {
        return ERR_NO_DIMENSIONS;
    }

    int ret = ERR_NO_ERROR;
    Morphyp mi = (Morphyp)m;
    
    ret = mpl_set_numtaxa(ntax, mi);
    if (ret) {
        return ret;
    }
    ret = mpl_set_num_charac(nchar, mi);
    if (ret) {
        return ret;
    }
    
    mi->symbols.statesymbols = NULL;

    return ret;
}

//int     mpl_get_numtaxa(Morphy m);
int mpl_get_numtaxa(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    return ((Morphyp)m)->numtaxa;
}


//int     mpl_get_num_charac(Morphy m);
int mpl_get_num_charac(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    return ((Morphyp)m)->numcharacters;
}

//int     mpl_attach_symbols(char* symbols, Morphy m);
int mpl_attach_symbols(const char *symbols, Morphy m)
{
    if (!symbols || !m) {
        return ERR_BAD_PARAM;
    }
    
    int isdataloaded = mpl_check_data_loaded((Morphyp)m);
    
    int i = 0;
    int len = 0;
    
    while(isalnum(symbols[i])) {
        ++len;
        ++i;
    };
    ++len;
    
    char* symbsnospaces = (char*)calloc(len, sizeof(char));
    
    int j = 0;
    for (i = 0; symbols[i]; ++i) {
        if (isalnum(symbols[i])) {
            symbsnospaces[j] = symbols[i];
            ++j;
        }
    }
    symbsnospaces[j] = '\0';
    
    printf("Symbols after removal of spaces: %s\n", symbsnospaces);
    
    if (isdataloaded) {
        char* matrixsymbs = mpl_query_symbols_from_matrix((Morphyp)m);
        // TODO: Get the matrix symbols
        assert(matrixsymbs);
        dbg_printf("Data is loaded already...\n");
        if (mpl_compare_symbol_lists(symbsnospaces, matrixsymbs)) {
            dbg_printf("Bad symbols...\n");
            free(symbsnospaces);
            return ERR_SYMBOL_MISMATCH;
        }
    }

    ((Morphyp)m)->symbols.statesymbols = symbsnospaces;
    
    return ERR_NO_ERROR;
}

char* mpl_get_symbols(Morphy m)
{
// TODO: Implement get_symbols
    Morphyp mi = (Morphyp)m;
    
    return mi->symbols.statesymbols;
}


//int     mpl_attach_rawdata(const char* rawmatrix, const Morphy m);
int mpl_attach_rawdata(const char* rawmatrix, Morphy m)
{
    if (!rawmatrix || !m) {
        return ERR_BAD_PARAM;
    }
    
    if (!mpl_get_numtaxa(m) || !mpl_get_num_charac(m)) {
        return ERR_NO_DIMENSIONS;
    }
    
    
    
    Morphyp m1 = (Morphyp)m;
    mpl_copy_raw_matrix(rawmatrix, m1);
    
    // Check validity of preprocessed matrix
    MPL_ERR_T err = ERR_NO_ERROR;
    err = mpl_check_nexus_matrix_dimensions(mpl_get_preprocessed_matrix(m1),
                                                mpl_get_numtaxa(m1),
                                                mpl_get_num_charac(m1));
    
    if (err) {
        // Cleanup; return error
        dbg_printf("Invalid data :(\n");
        mpl_delete_rawdata(m1);
        return err;
    }
    
    err = mpl_convert_rawdata((Morphyp)m);
    
    return err;
}

//int     mpl_delete_rawdata(Morphy m);
int mpl_delete_rawdata(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    Morphyp mp = (Morphyp)m;
    
    // TODO: This *must* reset all matrix dependencies
    if (mp->char_t_matrix) {
        free(mp->char_t_matrix);
        mp->char_t_matrix = NULL;
        mpl_delete_mpl_matrix(mpl_get_mpl_matrix((Morphyp)m));
        mp->inmatrix = NULL;
    }
    return ERR_NO_ERROR;
}


//int     mpl_set_postorder(const int nodeID, const int index, Morphy m);
//
//int     mpl_incl_charac(const int charID, Morphy m);
//int     mpl_excl_charac(const int charID, Morphy m);
//
//int     mpl_set_charac_weight(const int charID, Mflt weight);
//
//int     mpl_set_parsim_t(const int charID, const ptype_t parsfxn, Morphy m);

//int     mpl_set_gaphandl(const gap_t gaptype, Morphy m);
// TODO: Document gap_t
int mpl_set_gaphandl(const gap_t gaptype, Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    Morphyp mp = (Morphyp)m;
    mp->gaphandl = gaptype;
    return ERR_NO_ERROR;
}

int mpl_query_gaphandl(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    return mpl_get_gaphandl((Morphyp)m);
}
//
//int     mpl_down_recon(const int nodeID, const int lChild, const int rChild, Morphy m);
//int     mpl_up_recon(const int nodeID, const int lChild, const int rChild, const int parentID, Morphy m);
//int     mpl_up_final_recon(const int nodeID, const int lChild, const int rChild, const int parentID, Morphy m);
//int     mpl_update_tip(const int nodeID, Morphy m);
//int     mpl_finalize_tip(const int nodeID, Morphy m);
//
//int     mpl_get_insertcost_max(const int srcID, const int tgt1ID, const int tgt2ID, Morphy m);
//int     mpl_get_insertcost_min(const int srcID, const int tgt1ID, const int tgt2ID, Morphy m);
//
//int     mpl_get_packed_states(const int nodeID, const int character, int passnum, Morphy m);
//char*   mpl_get_stateset(const int nodeID, const int character, int passnum, Morphy m);
