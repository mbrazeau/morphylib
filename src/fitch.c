//
//  fitch.c
//  MorPhy2
//
//  Created by mbrazeau on 02/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "morphydefs.h"
#include "mplerror.h"
#include "morphy.h"
#include "fitch.h"
#include "statedata.h"

/**/
int mpl_fitch_downpass
(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int steps = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* left  = lset->prelimset;
    MPLstate* right = rset->prelimset;
    MPLstate* n     = nset->prelimset;
    
    for (i = 0; i < nchars; ++i) {
        j = indices[i];
        if (left[j] & right[j]) {
            n[j] = left[j] & right[j];
        }
        else {
            n[i] = left[j] | right[j];
            ++steps;
        }
    }
    
    return steps;
}


int mpl_fitch_uppass
(MPLstatesets* lset,
 MPLstatesets* rset,
 MPLstatesets* nset,
 MPLstatesets* anc,
 MPLpartition* part)
{
    
    return -1;
}


/**/
int mpl_NA_fitch_first_downpass
(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLpartition* part)
{
    
    return -1;
}


int mpl_NA_fitch_first_uppass
(MPLstatesets* lset,
 MPLstatesets* rset,
 MPLstatesets* nset,
 MPLstatesets* anc,
 MPLpartition* part)
{
    
    return -1;
}


int mpl_NA_fitch_second_downpass
(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLpartition* part)
{
    int steps = 0;
    
    return steps;
}


int mpl_NA_fitch_second_uppass
(MPLstatesets* lset,
 MPLstatesets* rset,
 MPLstatesets* nset,
 MPLstatesets* anc,
 MPLpartition* part)
{
    int steps = 0;
    
    return steps;
}

