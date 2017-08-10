//
//  testmpl.c
//  MorPhy2
//
//  Created by mbrazeau on 12/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mpltest.h"
#include "testmpl.h"

int test_create_destroy_Morphy(void)
{
    theader("Test simple create/destroy");
    int failn = 0;
    
    Morphy new = mpl_new_Morphy();
    if (!new) {
        ++failn;
        pfail;
        return failn;
    }
    else {
        ppass;
    }
    
    if (mpl_delete_Morphy(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    new = NULL;
    if (!mpl_delete_Morphy(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}


int test_init_Morphy(void)
{
    theader("Test of basic Morphy initialisation");
    int err     = 0;
    int failn   = 0;
    
    Morphy new = mpl_new_Morphy();
    
    int ntax = 21;
    int nchar = 100;
    
    err = mpl_init_Morphy(ntax, nchar, new);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (ntax != mpl_get_numtaxa(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (nchar != mpl_get_num_charac(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(new);
    
    return failn;
}


int test_reinit_no_data(void)
{
    theader("Reinitialise the dimensions with no dataset loaded");
    int err     = 0;
    int failn   = 0;
    
    /* The code of your test */

    Morphy new = mpl_new_Morphy();
    
    int ntax = 21;
    int nchar = 100;
    
    err = mpl_init_Morphy(ntax, nchar, new);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    ntax = 10;
    nchar = 200;
    
    err = mpl_init_Morphy(ntax, nchar, new);
    
    if (ntax != mpl_get_numtaxa(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (nchar != mpl_get_num_charac(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(new);
    
    return failn;
}

int test_attemp_load_bad_dimens(void)
{
    theader("Attempt to load data with wrong dimensions");
    int err     = 0;
    int failn   = 0;
    
    /* The code of your test */
    
    Morphy new = mpl_new_Morphy();
    
    int ntax = 5;
    int nchar = 10;
    
    char *rawmatrix =
    "0000000010\
    0-001-22-0\
    0-001-110-\
    10(03)0101100\
    1-000-0000\
    0-00{01}100-0;";
    
    err = mpl_init_Morphy(ntax, nchar, new);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    err = mpl_attach_rawdata(rawmatrix, new);
    
    if (!err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    // Correct ntax:
    ntax = 6;
    
    err = mpl_init_Morphy(ntax, nchar, new);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    // Now attach the data:
    // This should return no error.
    err = mpl_attach_rawdata(rawmatrix, new);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(new);
    return failn;

}

int test_reinit_with_data(void)
{
    theader("Reinitialise the dimensions with a dataset loaded");
    int err     = 0;
    int failn   = 0;
    
    /* The code of your test */
    
    Morphy new = mpl_new_Morphy();
    
    int ntax = 6;
    int nchar = 10;
    
    char *rawmatrix =
    "0000000010\
    0-001-22-0\
    0-001-110-\
    10(03)0101100\
    1-000-0000\
    0-00{01}100-0;";
    
    err = mpl_init_Morphy(ntax, nchar, new);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    err = mpl_attach_rawdata(rawmatrix, new);
    
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
   
    ntax = 10;
    nchar = 200;
    
    err = mpl_init_Morphy(ntax, nchar, new);
    if (!err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (ntax == mpl_get_numtaxa(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (nchar == mpl_get_num_charac(new)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(new);
    
    return failn;
}


int test_simple_chtype_setting(void)
{
    theader("Simple test of setting character types (parsimony)");
    int err     = 1;
    int failn   = 0;
    
    /* The code of your test */
    Morphy m = mpl_new_Morphy();
    mpl_init_Morphy(5, 10, m);
    err = mpl_set_parsim_t(0, DOLLO_T, m);
    
    if (err != ERR_CASE_NOT_IMPL) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    err = mpl_set_parsim_t(100, FITCH_T, m);
    if (err != ERR_OUT_OF_BOUNDS) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    err = mpl_set_parsim_t(5, MAX_CTYPE, m);
    if (err != ERR_UNKNOWN_CHTYPE) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    err = mpl_set_parsim_t(1, FITCH_T, m);
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m);
    return failn;
}

int test_delete_Morphy_no_input(void)
{
    theader("Testing Morphy deletion with no input handle")
    int err         = 0;
    int failn       = 0;
    Morphy handle   = NULL;
    
    err = mpl_delete_Morphy(handle);
    
    if (!err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_deletion_success(void)
{
    theader("Testing success of Morphy deletion");
    int err     = 1;
    int failn   = 0;
    
    /* The code of your test */
    if (err) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_inapplic_state_restoration(void)
{
    theader("Testing restoration of character state sets after reoptimisation");
    int err     = 0;
    int failn   = 0;
    
    // Do basic interface test (i.e. pass in NULL object ptr)
    
    int ntax = 8;
    int nchar = 10;
    
    Morphy m = NULL;
//   1234567890
    char* matrix =
    "12100-0-00\
     -212------\
     ----------\
     1----11111\
     2----1-1-1\
     0-0010--1-\
     -11111111-\
     0111111111;";
    
    err = mpl_init_Morphy(ntax, nchar, m);
    if (err != ERR_UNEXP_NULLPTR) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    m = mpl_new_Morphy();
    err = mpl_init_Morphy(ntax, nchar, m);
    
    if (err < 0 ) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_set_num_internal_nodes(ntax, m);
    mpl_attach_rawdata(matrix, m);
    mpl_apply_tipdata(m);
    
    // Build a tree
    char * treenwk = "(1,(2,((3,4),(5,(7,(6,8))))));";
    TLP tlp = tl_new_TL();
    tl_set_numtaxa(ntax, tlp);
    tl_attach_Newick(treenwk, tlp);
    tl_set_current_tree(0, tlp);
    TLtree *tree = tl_get_TLtree(tlp);
    
    // Allocate storage for state sets:
    unsigned int beforestates1 [2 * ntax - 1][nchar];
    unsigned int beforestates2 [2 * ntax - 1][nchar];
    unsigned int beforestates3 [2 * ntax - 1][nchar];
    unsigned int beforestates4 [2 * ntax - 1][nchar];
    
    unsigned int afterstates1 [2 * ntax - 1][nchar];
    unsigned int afterstates2 [2 * ntax - 1][nchar];
    unsigned int afterstates3 [2 * ntax - 1][nchar];
    unsigned int afterstates4 [2 * ntax - 1][nchar];
    
    
    // Optimize data on the tree
    int origlen = 0;
    origlen = test_do_fullpass_on_tree(tree, m);
    
    // Remove a branch
    TLnode* orig = NULL;
    orig = tl_remove_branch(&tree->trnodes[10], tree);

    int rttreelen = 0;
    rttreelen = test_do_fullpass_on_tree(tree, m);
    
    // Show 'before' state sets
    int i = 0;
    for (i = 0; i < (2 * ntax -1); ++i) {
        int k = 0;
        for (k = 0; k < nchar; ++k) {
            beforestates1[i][k] = mpl_get_packed_states(i, k, 1, m);
            beforestates2[i][k] = mpl_get_packed_states(i, k, 2, m);
            beforestates3[i][k] = mpl_get_packed_states(i, k, 3, m);
            beforestates4[i][k] = mpl_get_packed_states(i, k, 4, m);
        }
    }
    
    for (i = 0; i < (2 * ntax - 1); ++i) {
        printf("Node %i:\n", i);
        int j = 0;
        int k = 0;
        printf("char num:\t");
        for (k = 0; k < nchar; ++k) {
            printf("%i ", k);
        }
        printf("\n");
        for (j = 1; j < 5; ++j) {
            printf("Pass number %i: ", j);
            for (k = 0; k < nchar; ++k) {
                printf("%s ", mpl_get_stateset(i, k, j, m));
            }
            printf("\n");
        }
        printf("\n");
    }
    
    // Reoptimize the subtree
    
    // TODO: Now, need to perturb the tree without touching the temp state storage
    // First attempt a local reoptimization and store characters needing update
    int addlen = 0;
    addlen = mpl_get_insertcost(tree->trnodes[10].index, orig->index, orig->anc->index, false, 100000, m);
    int doreopt = 0;
    doreopt = mpl_check_reopt_inapplics(m);
    
    test_partial_downpass_for_inapplicables(tree, m);
    
    for (i = 0; i < (2 * ntax -1); ++i) {
        int k = 0;
        for (k = 0; k < nchar; ++k) {
            afterstates1[i][k] = mpl_get_packed_states(i, k, 1, m);
            afterstates2[i][k] = mpl_get_packed_states(i, k, 2, m);
            afterstates3[i][k] = mpl_get_packed_states(i, k, 3, m);
            afterstates4[i][k] = mpl_get_packed_states(i, k, 4, m);
        }
    }
    // Test 'after' state sets
    int mismatches = 0;
    for (i = 0; i < (2 * ntax -1); ++i) {
        int k = 0;
        for (k = 0; k < nchar; ++k) {
            if (beforestates1[i][k] != afterstates1[i][k]) {
                ++mismatches;
            }
            if (beforestates2[i][k] != afterstates2[i][k]) {
                ++mismatches;
            }
            if (beforestates3[i][k] != afterstates3[i][k]) {
                ++mismatches;
            }
            if (beforestates4[i][k] != afterstates4[i][k]) {
                ++mismatches;
            }
        }
    }
    
    if (mismatches == 0) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    // Apply state set restoration
    for (i = 0; i < (2* ntax - 1); ++i) {
        mpl_restore_original_sets(i, m);
    }
    // Show the restored state sets.
    
    for (i = 0; i < (2 * ntax -1); ++i) {
        int k = 0;
        for (k = 0; k < nchar; ++k) {
            afterstates1[i][k] = mpl_get_packed_states(i, k, 1, m);
            afterstates2[i][k] = mpl_get_packed_states(i, k, 2, m);
            afterstates3[i][k] = mpl_get_packed_states(i, k, 3, m);
            afterstates4[i][k] = mpl_get_packed_states(i, k, 4, m);
        }
    }
    
    for (i = 0; i < (2 * ntax - 1); ++i) {
        printf("Node %i:\n", i);
        int j = 0;
        int k = 0;
        printf("char num:\t");
        for (k = 0; k < nchar; ++k) {
            printf("%i ", k);
        }
        printf("\n");
        for (j = 1; j < 5; ++j) {
            printf("Pass number %i: ", j);
            for (k = 0; k < nchar; ++k) {
                printf("%s ", mpl_get_stateset(i, k, j, m));
            }
            printf("\n");
        }
        printf("\n");
    }
    // TODO: Once this is done, store state sets in arrays and compare them
    // without printing them to the buffer.
    
    for (i = 0; i < (2 * ntax -1); ++i) {
        int k = 0;
        for (k = 0; k < nchar; ++k) {
            if (beforestates1[i][k] != afterstates1[i][k]) {
                ++failn;
                pfail;
            }
            else {
                ppass;
            }
            if (beforestates2[i][k] != afterstates2[i][k]) {
                ++failn;
                pfail;
            }
            else {
                ppass;
            }
            if (beforestates3[i][k] != afterstates3[i][k]) {
                ++failn;
                pfail;
            }
            else {
                ppass;
            }
            if (beforestates4[i][k] != afterstates4[i][k]) {
                ++failn;
                pfail;
            }
            else {
                ppass;
            }
        }
    }
    return failn;
}
