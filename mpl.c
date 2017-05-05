//
//  mpl.c
//  MorPhy2
//
//  Created by mbrazeau on 04/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mplerror.h"
#include "mpl.h"
#include "morphy.h"
#include "statedata.h"


/*!
 @brief Creates a new instance of a Morphy object
 ------
 @discussion Creates a new empty Morphy object. All fields are unpopulated and
 uninitialised.
 @return A void pointer to the Morphy instance. NULL if unsuccessful.
 */
Morphy mpl_new_Morphy(void)
{
    Morphyp new     = mpl_new_Morphy_t();
    if (new) {
        new->symboldict = mpl_alloc_symbolset();
    }
    
    return (Morphy)new;
}


/*!
 @brief Destroys an instance of a Morphy object.
 ------
 @discussion Destroys an instance of the Morphy object, calling all destructors
 for internal object completely returning the memory to the system.
 @param m An Morphy object to be destroyed.
 @return A Morphy error code.
 */
int mpl_delete_Morphy(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    Morphyp m1 = (Morphyp)m;
    
    // TODO: All Morphy destructors
    mpl_delete_rawdata(m);
    mpl_destroy_symbolset(m1->symboldict);
    mpl_delete_mpl_matrix(m1->inmatrix);
    free(m1);
    
    return ERR_NO_ERROR;
}

//int     mpl_set_numtaxa(const int ntax, Morphy m);
int mpl_set_numtaxa(const int ntax, Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    ((Morphyp)m)->numtaxa = ntax;
    
    return ERR_NO_ERROR;
}

//int     mpl_get_numtaxa(Morphy m);
int mpl_get_numtaxa(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    return ((Morphyp)m)->numtaxa;
}

//int     mpl_set_num_charac(const int nchar, Morphy m);
int mpl_set_num_charac(const int nchar, Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    ((Morphyp)m)->numcharacters = nchar;
    
    return ERR_NO_ERROR;
}

//int     mpl_get_num_charac(Morphy m);
int mpl_get_num_charac(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    return ((Morphyp)m)->numtaxa;
}

//int     mpl_attach_symbols(char* symbols, Morphy m);
int mpl_attach_symbols(const char *symbols, Morphy m)
{
    if (!symbols || !m) {
        return ERR_BAD_PARAM;
    }
    // TODO: Implement attach_symbols
    // Copy symbols; remove spaces and punc
    // Allocate memory for symbols
    // Attach symbols
    
    return ERR_NO_ERROR;
}

char* mpl_get_symbols(Morphy m)
{
// TODO: Implement get_symbols
    Morphyp mi = (Morphyp)m;
    
    return mi->symboldict->rawsymbols;
}


//int     mpl_attach_rawdata(const char* rawmatrix, const Morphy m);
int mpl_attach_rawdata(const char* rawmatrix, Morphy m)
{
    if (!rawmatrix || !m) {
        return ERR_BAD_PARAM;
    }
    
    Morphyp m1 = (Morphyp)m;
    
    if (!mpl_get_numtaxa(m) || !mpl_get_num_charac(m)) {
        return ERR_NO_DIMENSIONS;
    }
    
    mpl_copy_raw_matrix(rawmatrix, m1);
    
    // Check validity of preprocessed matrix
    MPL_ERR_T invalid = mpl_check_nexus_matrix_dimensions
    (mpl_get_preprocessed_matrix(m1),
     mpl_get_numtaxa(m1),
     mpl_get_num_charac(m1));
    
    if (invalid) {
        // Cleanup; return error
        mpl_delete_rawdata(m1);
        return invalid;
    }
    
    mpl_convert_rawdata((Morphyp)m);
    
    return ERR_NO_ERROR;
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
