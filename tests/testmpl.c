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
