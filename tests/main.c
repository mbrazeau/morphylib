//
//  main.c
//  MorPhy2
//
//  Created by mbrazeau on 09/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include <stdio.h>
#include "mpltest.h"
#include "teststatedata.h"
#include "testmorphy.h"
#include "testmpl.h"
#include "testfitch.h"
#include "testwagner.h"

int main (void)
{
    int fails = 0;
    
    // statedata.c tests
    fails += test_get_states();
    fails += test_count_applic_inapplic();
    fails += test_good_symbols();
    fails += test_bad_symbols();
    fails += test_load_symbols();
    fails += test_bad_load_symbols();
    fails += test_multistate_symbols();
    fails += test_usr_order_symbols();
    fails += test_big_multistate_symbols();
    
    // morphy.c tests
    fails += test_isreal();
    fails += test_almost_equal();
    fails += test_resetting_weights();
    fails += test_resetting_frac_weights();
    fails += test_count_gaps_basic();
    fails += test_partition_push_index();
    fails += test_data_partitioning_simple();
    fails += test_data_partitioning_gapmissing();
    fails += test_data_partitioning_gapnewstate();
    fails += test_weights_realtree();
    fails += test_set_weights();
    
    // mpl.c tests
    fails += test_create_destroy_Morphy();
    fails += test_simple_chtype_setting();
    fails += test_init_Morphy();
    fails += test_reinit_no_data();
    fails += test_reinit_with_data();
    fails += test_attemp_load_bad_dimens();
    fails += test_delete_Morphy_no_input();
    fails += test_basic_tip_apply();
    // TODO: set this test up to return
    test_state_retrieval();
    
    // fitch.c tests
    fails += test_small_fitch();
    fails += test_weighted_small_fitch();
    fails += test_small_fitch_na();
    fails += test_small_fitch_withmissing();
    fails += test_bulk_balanced_tree_cases();
    fails += test_bulk_unrooted_tree_cases();
    fails += test_twopass_fitch();
    fails += test_local_reoptimisation();
    fails += test_get_partial_reopt_for_na();
    
    // wagner.c tests 
    fails += test_small_wagner();
    fails += test_wagner_extended();
    
    printf("\n\nTest summary:\n\n");
    if (fails) {
        psumf(fails);
    }
    else {
        psump;
    }
    printf("\n");
    
    return fails;
}
