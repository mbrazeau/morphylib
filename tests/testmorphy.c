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
