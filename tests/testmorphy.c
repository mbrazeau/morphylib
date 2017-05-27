//
//  testmorphy.c
//  MorPhy2
//
//  Created by mbrazeau on 12/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mpltest.h"
//#include "testmorphy.h"
#include "../src/morphy.h"

int test_isreal(void)
{
    theader("Testing check for real numbers");
//    int err     = 0;
    int failn   = 0;
    
    /* The code of your test */
    double whole1 = 1.0;
    double whole2 = 2.0;
    double whole3 = 15.0;
    double whole4 = 23.0;
    double frac1 = 1.5;
    double frac2 = 2.789;
    double frac3 = 3.0000001;
    double frac4 = 4.000000002;
    
    if (mpl_isreal(whole1)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (mpl_isreal(whole2)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (mpl_isreal(whole3)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (mpl_isreal(whole4)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (!mpl_isreal(frac1)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (!mpl_isreal(frac2)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (!mpl_isreal(frac3)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (!mpl_isreal(frac4)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_almost_equal(void)
{
    theader("Testing almost equal function for comparing doubles");
    int failn   = 0;
    
    /* The code of your test */
    double athird1 = 0.3333333;
    double athird2 = 0.3333333;
    double athird3 = 0.3333334;
    double frac1 = 1.2345678;
    double frac2 = 1.2346789;
    
    if (!mpl_almost_equal(athird1, athird2)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (mpl_almost_equal(athird1, athird3)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (mpl_almost_equal(frac1, frac2)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_resetting_weights(void)
{
    theader("Testing a two-tip weighted Fitch with weight reset");
    
    int failn = 0;
    int ntax	= 2;
    int nchar	= 10;
    //    int numna   = 0;
    char *rawmatrix =
    "0000000010\
    1111111111;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_charac_weight(1, 3, m1);
    mpl_set_num_internal_nodes(1, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    length = mpl_first_down_recon(2, 1, 0, m1);
    printf("The length: %i\n", length);
    
    if (length != 11) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_set_charac_weight(1, 1, m1);
    mpl_apply_tipdata(m1);
    
    length = mpl_first_down_recon(2, 1, 0, m1);

    if (length != 9) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_set_charac_weight(1, 0.0, m1);
    mpl_set_charac_weight(2, 0.0, m1);
    mpl_set_charac_weight(3, 0.0, m1);
    mpl_apply_tipdata(m1);
    
    length = mpl_first_down_recon(2, 1, 0, m1);

    if (length != 6) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_set_charac_weight(1, 1, m1);
    mpl_set_charac_weight(2, 1, m1);
    mpl_set_charac_weight(3, 1, m1);
    mpl_apply_tipdata(m1);
    
    length = mpl_first_down_recon(2, 1, 0, m1);

    if (length != 9) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    mpl_delete_Morphy(m1);
    
    return failn;
}


int test_resetting_frac_weights(void)
{
    theader("Testing resetting of weights after using real numbers");
    
    int failn = 0;
    int ntax	= 2;
    int nchar	= 10;
    //    int numna   = 0;
    char *rawmatrix =
    "0000000010\
     1111111111;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_charac_weight(1, 0.333, m1);
    mpl_set_charac_weight(2, 0.333, m1);
    mpl_set_charac_weight(7, 0.333, m1);
    mpl_set_num_internal_nodes(1, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    length = mpl_first_down_recon(2, 1, 0, m1);
    printf("The length: %i\n", length);
    
    if (length != 21) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_set_charac_weight(1, 1, m1);
    mpl_set_charac_weight(2, 1, m1);
    mpl_set_charac_weight(7, 1, m1);
    mpl_apply_tipdata(m1);
    
    length = mpl_first_down_recon(2, 1, 0, m1);

    if (length != 9) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    mpl_delete_Morphy(m1);
    
    return failn;
}



int test_count_gaps_basic(void)
{
    theader("Testing counting of gaps in matrix");
    int failn = 0;
    int ntax	= 6;
    int nchar	= 10;
    int numna   = 0;
    char *rawmatrix =
"0000000010\
0-001-22-0\
0-001-110-\
10(03)0101100\
1-000-0000\
0-00{01}100-0;";

    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_attach_rawdata(rawmatrix, m1);

    numna = mpl_count_gaps_in_columns((Morphyp)m1);
    
    if (numna == 2) {
        ppass;
    } else {
        ++failn;
        pfail;
    }
    mpl_delete_Morphy(m1);
    
    return failn;

}

int test_partition_push_index(void)
{

    theader("Testing new index pushing in partitioning");
    int failn = 0;
    
    MPLpartition* part = mpl_new_partition(FITCH_T, false);
    if (!part) {
        ++failn;
        pfail;
        return failn;
    }
    
    int nindices    = 4;
    int indices[]   = {10, 32, 21, 110};
    int i = 0;
    
    for (i = 0; i < nindices; ++i) {
        mpl_part_push_index(indices[i], part);
    }
    
    for (i = 0; i < nindices; ++i) {
        if (indices[i] != part->charindices[i]) {
            ++failn;
            pfail;
        }
        else {
            ppass;
        }
    }
    
    if (part->ncharsinpart != nindices) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (part->ncharsinpart != part->maxnchars) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_partition(part);
    
    return failn;
}

int test_data_partitioning_simple(void)
{
    theader("Testing counting of gaps in matrix");
    int failn = 0;
    int ntax	= 6;
    int nchar	= 10;
//    int numna   = 0;
    char *rawmatrix =
"0000000010\
0-001-22-0\
0-001-110-\
10(03)0101100\
1-000-0000\
0-00{01}100-0;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    mpl_apply_tipdata(m1);
//    mpl_setup_partitions((Morphyp)m1);
//
//    Morphyp m = (Morphyp)m1;
    int nparts = mpl_get_numparts((Morphyp)m1);
    
    if (nparts != 2) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m1);
    
    return failn;
    
}


int test_data_partitioning_gapmissing(void)
{
    theader("Testing counting of gaps in matrix");
    int failn = 0;
    int ntax	= 6;
    int nchar	= 10;
//    int numna   = 0;
    char *rawmatrix =
    "0000000010\
    0-001-22-0\
    0-001-110-\
    10(03)0101100\
    1-000-0000\
    0-00{01}100-0;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_attach_rawdata(rawmatrix, m1);
        mpl_set_gaphandl(GAP_MISSING, m1);
    mpl_setup_partitions((Morphyp)m1);
    //
    //    Morphyp m = (Morphyp)m1;
    int nparts = mpl_get_numparts((Morphyp)m1);
    
    if (nparts != 1) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m1);
    
    return failn;
    
}

int test_data_partitioning_gapnewstate(void)
{
    theader("Testing counting of gaps in matrix");
    int failn = 0;
    int ntax	= 6;
    int nchar	= 10;
//    int numna   = 0;
    char *rawmatrix =
    "0000000010\
    0-001-22-0\
    0-001-110-\
    10(03)0101100\
    1-000-0000\
    0-00{01}100-0;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    mpl_set_gaphandl(GAP_NEWSTATE, m1);
    mpl_setup_partitions((Morphyp)m1);
    //
    //    Morphyp m = (Morphyp)m1;
    int nparts = mpl_get_numparts((Morphyp)m1);
    
    if (nparts != 1) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m1);
    
    return failn;
    
}

int test_set_weights(void)
{
    theader("Test of basic weight setting");
//    int err     = 0;
    int err = 0;
    int failn   = 0;
    
    /* The code of your test */
    int ntax = 6;
    int nchar = 10;
    double weights[] = {0.333, 0.25, 0.5};
    int chars2wt[] = {2, 5, 6};
    int i = 0;
    int numwts = 3;
 
    char *rawmatrix =
    "0000000010\
    0-001-22-0\
    0-001-110-\
    10(03)0101100\
    1-000-0000\
    0-00{01}100-0;";
    
    Morphy m = mpl_new_Morphy();
    
    mpl_init_Morphy(ntax, nchar, m);

    mpl_attach_rawdata(rawmatrix, m);
    for (i = 0; i < numwts; ++i) {
        mpl_set_charac_weight(chars2wt[i], weights[i], m);
    }
    
    Morphyp mi = (Morphyp)m;
    mpl_scale_all_intweights(mi);
    err = mpl_apply_tipdata(mi);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    
    mpl_delete_Morphy(m);
    // TODO: Give test a real return value
    return 0;
}

int test_weights_realtree(void)
{
    theader("Testing weights on a real tree");
    //    int err     = 0;
    int failn   = 0;
    
    int ntax	= 12;
    int nchar	= 2;
    char *rawmatrix =
    "213-----11?1?1-1--0-3-21;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    mpl_set_num_internal_nodes(13, m1);
    mpl_set_charac_weight(0, 1, m1);
    mpl_set_charac_weight(1, 2, m1);
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    int tipancs[]= {12, 12, 13, 14, 15, 16, 21, 20, 19, 18, 17, 17};
    int ancs[]   = {13, 14, 15, 16, 22, 18, 19, 20, 21, 22, 23};
    int nodes[]  = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    int ldescs[] = {0,  12, 13, 14, 15, 10,  9,  8,  7,  6, 16};
    int rdescs[] = {1,   2,  3,  4,  5, 11, 17, 18, 19, 20, 21};
    
    // ...
    int i = 0;
    for (i = 0; i < (ntax-1); ++i) {
        length += mpl_first_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    // Update lower root
    mpl_update_lower_root(23, 22, m1);
    
    for (i = (ntax-2); i >= 0; --i) {
        length += mpl_first_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    for (i = 0; i < ntax; ++i) {
        mpl_update_tip(i, tipancs[i], m1);
    }
    
    for (i = 0; i < (ntax-1); ++i) {
        // Second downpass
        length += mpl_second_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    for (i = (ntax-2); i >= 0; --i) {
        // Second uppass reconstruction
        length += mpl_second_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    
    if (length != 9) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    
    mpl_delete_Morphy(m1);
    
    return failn;
}

int test_basic_tip_apply(void)
{
    theader("Testing application of state data to tips");
    int failn = 0;
    int ntax	= 6;
    int nchar	= 10;
    //    int numna   = 0;
    char *rawmatrix =
    "0000000010\
    0-001-22-0\
    0-001-110-\
    10(03)0101100\
    1-000-0000\
    0-00{01}100-0;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    //    mpl_set_gaphandl(GAP_NEWSTATE, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    mpl_apply_tipdata(m1);
    
    Morphyp mi = (Morphy)m1;
    
    int i = 0;
    int j = 0;
    for (i = 0; i < ntax; ++i) {
        printf("\t");
        for (j = 0; j < nchar; ++j) {
            
            MPLstate res = mi->statesets[i]->downpass1[j];
            if (res == (~0)) {
                printf("%c\t", '?');
            }
            else {
                printf("%u\t", res);
            }
        }
        printf("\n");
    }
    printf("\n");
    
    mpl_delete_Morphy(m1);
    
    failn = 0;
    
    return failn;
    
}

