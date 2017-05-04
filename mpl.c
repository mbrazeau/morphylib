//
//  mpl.c
//  MorPhy2
//
//  Created by mbrazeau on 04/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include <string.h>
#include "mplerror.h"
#include "mpl.h"
#include "morphy.h"
#include "statedata.h"

//Morphy  mpl_new_Morphy(void);
Morphy mpl_new_Morphy(void)
{
    Morphyp new = mpl_new_Morphy_t();
    return (Morphy)new;
}

//int     mpl_delete_Morphy(Morphy m);
int mpl_delete_Morphy(Morphy m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    Morphyp m1 = (Morphyp)m;
    
    // TODO: Morphy destructors
    mpl_delete_rawdata(m);
    
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

//int     mpl_attach_rawdata(char* rawmatrix, Morphy m);
int mpl_attach_rawdata(char* rawmatrix, Morphy m)
{
    if (!rawmatrix || !m) {
        return ERR_BAD_PARAM;
    }
    
    char *matcpy = NULL;
    unsigned long len = 1 + strlen(rawmatrix);
    
    matcpy = (char*)malloc(len * sizeof(char));
    if (!matcpy) {
        return ERR_BAD_MALLOC;
    }
    
    strcpy(matcpy, rawmatrix);
    ((Morphyp)m)->char_t_matrix = matcpy;
    
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
