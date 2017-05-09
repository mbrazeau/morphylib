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

int main (void)
{
    int fails = 0;
    
    fails += test_get_states();
    fails += test_count_applic_inapplic();
    fails += test_good_symbols();
    fails += test_bad_symbols();
    fails += test_load_symbols();
    fails += test_bad_load_symbols();
    fails += test_multistate_symbols();
    fails += test_usr_order_symbols();
    
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
