//
//  testwagner.c
//  morphylib
//
//  Created by mbrazeau on 21/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mpltest.h"
#include "mpl.h"
#include "morphydefs.h"
#include "wagner.h"
#include "testwagner.h"
#include <string.h>

int test_small_wagner(void)
{
    theader("A simple test of the Wagner counting procedure");
    int failn = 0;
    int ntax	= 12;
    int nchar	= 2;
    char *rawmatrix =
    "11\
    11\
    00\
    00\
    00\
    00\
    00\
    00\
    00\
    00\
    02\
    02;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    //    mpl_set_gaphandl(GAP_MISSING, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_num_internal_nodes(13, m1);
    mpl_set_parsim_t(1, WAGNER_T, m1);
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
//    int tipancs[]= {12, 12, 13, 14, 15, 16, 21, 20, 19, 18, 17, 17};
//    int ancs[]   = {12, 14, 15, 16, 22, 18, 19, 20, 21, 22, 23};
    int nodes[]  = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    int ldescs[] = {0,  12, 13, 14, 15, 10,  9,  8,  7,  6, 16};
    int rdescs[] = {1,   2,  3,  4,  5, 11, 17, 18, 19, 20, 21};
    
    // ...
    int i = 0;
    for (i = 0; i < (ntax-1); ++i) {
        length += mpl_first_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    if (length != 4) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m1);
    return failn;
}

int test_wagner_extended(void)
{
    theader("Extended test of Wagner optimisation");
    int err     = 0;
    int failn   = 0;
    
    int numtaxa = 6;
    int nchar = 2;
    TLP tlp = tl_new_TL();
    tl_set_numtaxa(numtaxa, tlp);
    
    char* matrix =
   "21\
    21\
    20\
    10\
    10\
    03;";
    
    Morphy m = mpl_new_Morphy();
    
    err = mpl_init_Morphy(numtaxa, nchar, m);
    if (err) {
        ++failn;
        pfail;
        err = 0;
    }
    else {
        ppass;
    }
    
    mpl_attach_rawdata(matrix, m);
    mpl_set_parsim_t(0, WAGNER_T, m);
    mpl_set_parsim_t(1, WAGNER_T, m);
    mpl_set_num_internal_nodes(numtaxa, m);
    mpl_apply_tipdata(m);
    
    char* newick = "(((1,2),3),(4,(5,6)));";
    
    tl_attach_Newick(newick, tlp);
    
    tl_set_current_tree(0, tlp);
    TLtree* tree = tl_get_TLtree(tlp);
    
    int* postorder = (int*)calloc(2 * numtaxa, sizeof(int));
    dbg_printf("\n");
    
    int length = 0;
    
    length = test_do_fullpass_on_tree(tree, m);
    
    if (length != 6) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    free(postorder);
    mpl_delete_Morphy(m);
    tl_delete_TL(tlp);
    
    return failn;
}
