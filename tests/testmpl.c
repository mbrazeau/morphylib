//
//  testmpl.c
//  MorPhy2
//
//  Created by mbrazeau on 12/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mpltest.h"
#include "testmpl.h"

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
