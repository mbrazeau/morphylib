//
//  testfitch.c
//  MorPhy2
//
//  Created by mbrazeau on 16/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "mpltest.h"
#include "../src/mpl.h"
#include "../src/fitch.h"
#include "testfitch.h"


int test_small_fitch(void)
{
    theader("Testing a two-tip Fitch optimisation");
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
    
    mpl_set_num_internal_nodes(1, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    length = mpl_first_down_recon(2, 1, 0, m1);
    printf("The length: %i\n", length);
    
    
    mpl_delete_Morphy(m1);
    
    if (length != 9) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}


int test_small_fitch_na(void)
{
    theader("A simple test of the NA counting procedure");
    int failn = 0;
    int ntax	= 12;
    int nchar	= 2;
    char *rawmatrix =
    "11\
    11\
    00\
    00\
    --\
    --\
    --\
    --\
    10\
    10\
    01\
    01;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    //    mpl_set_gaphandl(GAP_MISSING, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_num_internal_nodes(13, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    int tipancs[]= {12, 12, 13, 14, 15, 16, 21, 20, 19, 18, 17, 17};
    int ancs[]   = {12, 14, 15, 16, 22, 18, 19, 20, 21, 22, 23};
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
    
    for (i = (ntax-2); i; --i) {
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
    
    for (i = (ntax-2); i; --i) {
        // Second uppass reconstruction
        length += mpl_second_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    // TODO: Final tip calculations
    //    // Optional for basic length count, but probably necessary for
    //    // indirect optimisation.
    //    for (i = 0; i < ntax; ++i) {
    //        mpl_finalize_tip(i, tipancs[i], m1);
    //        // Update tips first time
    //    }
    
    mpl_delete_Morphy(m1);
    
    if (length != 6) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}
