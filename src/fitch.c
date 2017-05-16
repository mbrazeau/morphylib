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
(MPLndsets* lset, MPLndsets* rset, MPLndsets* nset, MPLpartition* part)
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
            n[j] = left[j] | right[j];
            ++steps;
        }
    }
    
    return steps;
}


int mpl_fitch_uppass
(MPLndsets* lset, MPLndsets* rset, MPLndsets* nset, MPLndsets* ancset,
 MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* left  = lset->prelimset;
    MPLstate* right = rset->prelimset;
    MPLstate* npre  = nset->prelimset;
    MPLstate* nfin  = nset->finalset;
    MPLstate* anc   = ancset->finalset;
    
    for (i = 0; i < nchars; ++i) {
        
        j = indices[i];
        
        if ((anc[j] & npre[j]) == anc[j]) {
            nfin[j] = anc[j] & npre[j];
        }
        else {
            if (left[j] & right[j]) {
                nfin[j] = (npre[j] | (anc[j] & (left[j] | right[j])));
            }
            else {
                nfin[j] = npre[j] | anc[j];
            }
        }
        
    }
    
    return 0;
}


/**/
int mpl_NA_fitch_first_downpass
(MPLndsets* lset, MPLndsets* rset, MPLndsets* nset, MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* left  = lset->NApass1;
    MPLstate* right = rset->NApass1;
    MPLstate* n     = nset->NApass1;
    MPLstate* stacts  = nset->subtree_actives;
    MPLstate* lacts   = lset->subtree_actives;
    MPLstate* racts   = rset->subtree_actives;
    MPLstate temp = 0;
    
    for (i = 0; i < nchars; ++i) {
        j = indices[i];
        
        if ((temp = (left[j] & right[j]))) {
            n[j] = temp;
            if (temp == NA) {
                if ((left[j] & ISAPPLIC) && (right[j] & ISAPPLIC)) {
                    n[j] = (left[j] | right[j]);
                }
            }
        }
        else {
            n[j] = (left[j] | right[j]);
            
            if ((left[j] & ISAPPLIC) && (right[j] & ISAPPLIC)) {
                n[j] = n[j] & ISAPPLIC;
            }
        }
        
        stacts[j] = (lacts[j] | racts[j]) & ISAPPLIC;
        
        assert(n[j]);
    }
    
    return 0;
}


int mpl_NA_fitch_first_uppass
(MPLndsets* lset, MPLndsets* rset, MPLndsets* nset, MPLndsets* ancset,
 MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* left  = lset->NApass1;
    MPLstate* right = rset->NApass1;
    MPLstate* npre  = nset->NApass1;
    MPLstate* nifin = nset->NApass2;
    MPLstate* anc   = ancset->NApass2;
    
    for (i = 0; i < nchars; ++i) {
        
        j = indices[i];
        
        if (npre[j] & NA) {
            if (npre[j] & ISAPPLIC) {
                if (anc[j] == NA) {
                    nifin[j] = NA;
                }
                else {
                    nifin[j] = npre[j] & ISAPPLIC;
                }
            }
            else {
                if (anc[j] == NA) {
                    nifin[j] = NA;
                }
                else {
                    if ((left[j] | right[j]) & ISAPPLIC) {
                        nifin[j] = ((left[j] | right[j]) & ISAPPLIC);
                    }
                    else {
                        nifin[j] = NA;
                    }
                }
            }
        }
        else {
            nifin[j] = npre[j];
        }
        
        assert(nifin[j]);
    }
    
    
    return 0;
}


int mpl_NA_fitch_second_downpass
(MPLndsets* lset, MPLndsets* rset, MPLndsets* nset, MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int steps = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* left  = lset->prelimset;
    MPLstate* right = rset->prelimset;
    MPLstate* nifin = nset->NApass2;
    MPLstate* npre    = nset->prelimset;
    MPLstate* stacts  = nset->subtree_actives;
    MPLstate* lacts   = lset->subtree_actives;
    MPLstate* racts   = rset->subtree_actives;
    MPLstate temp = 0;
    
    for (i = 0; i < nchars; ++i) {
        
        temp = 0;
        
        j = indices[i];
        
        if (nifin[j] & ISAPPLIC) {
            if ((temp = (left[j] & right[j]))) {
                npre[j] = temp & ISAPPLIC;
            }
            else {
                npre[j] = (left[j] | right[j]) & ISAPPLIC;
                
                if (left[j] & ISAPPLIC && right[j] & ISAPPLIC) {
                    ++steps;
                } else if (lacts[j] && racts[j]) {
                    ++steps;
                }
            }
        }
        else {
            npre[j] = nifin[j];
        }
        
        stacts[j] = (lacts[j] | racts[j]) & ISAPPLIC;
    
        assert(npre[j]);
    }
    
    
    return steps;
}


int mpl_NA_fitch_second_uppass
(MPLndsets* lset, MPLndsets* rset, MPLndsets* nset, MPLndsets* ancset,
 MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int steps = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* left  = lset->prelimset;
    MPLstate* right = rset->prelimset;
    MPLstate* npre  = nset->prelimset;
    MPLstate* nfin  = nset->finalset;
    MPLstate* anc   = ancset->finalset;
    MPLstate* lacts = lset->subtree_actives;
    MPLstate* racts = rset->subtree_actives;
    
    for (i = 0; i < nchars; ++i) {
        
        j = indices[i];
        
        if (npre[j] & ISAPPLIC) {
            if (anc[j] & ISAPPLIC) {
                if ((anc[j] & npre[j]) == anc[j]) {
                    nfin[j] = anc[j] & npre[j];
                } else {
                    if (left[j] & right[j]) {
                        nfin[j] = (npre[j] | (anc[j] & left[j] & right[j]));
                    }
                    else {
                        if ((left[j] | right[j]) & NA) {
                            if ((left[j] | right[j]) & anc[j]) {
                                nfin[j] = anc[j];
                            } else {
                                nfin[j] = (left[j] | right[j] | anc[j]) & NA;
                            }
                        } else {
                            nfin[j] = npre[j] | anc[j];
                            if ((anc[j] & nfin[j]) == anc[j]) {
                                nfin[j] = anc[j] & nfin[j];
                            }
                        }
                    }
                }
            }
            else {
                nfin[j] = npre[j];
            }
        }
        else {
            nfin[j] = npre[j];
            
            if (lacts[j] && racts[j]) {
                ++steps;
            }
        }
        assert(nfin[j]);
    }
    
    return steps;
}

int mpl_fitch_tip_update(MPLndsets* tset, MPLndsets* ancset, MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* tprelim = tset->prelimset;
    MPLstate* tfinal  = tset->finalset;
    MPLstate* astates = ancset->finalset;
    
    for (i = 0; i < nchars; ++i) {
        j = indices[i];
        if (tprelim[j] & astates[j]) {
            tfinal[j] = tprelim[j] & astates[j];
        }
        else {
            tfinal[j] = tprelim[j];
        }
        assert(tfinal[j]);
    }
    return 0;
}

int mpl_fitch_NA_tip_update
(MPLndsets* tset, MPLndsets* ancset, MPLpartition* part)
{
    int i     = 0;
    int j     = 0;
    int* indices    = part->charindices;
    int nchars      = part->ncharsinpart;
    MPLstate* tprelim = tset->NApass1;
    MPLstate* tfinal  = tset->prelimset;
    MPLstate* astates = ancset->NApass2;
    MPLstate* stacts  = tset->subtree_actives;
    
    for (i = 0; i < nchars; ++i) {
        
        j = indices[i];
        
        if (tprelim[j] & astates[j]) {
            stacts[j] = (tprelim[j] & astates[j] & ISAPPLIC);
        }
        else {
            stacts[j] |= tprelim[j] & ISAPPLIC;
        }
        
        
        
        tfinal[j] = tprelim[j];
        
        assert(tfinal[j]);
    }
    return 0;
}

int mpl_fitch_NA_tip_finalize
(MPLndsets* tset, MPLndsets* ancset, MPLpartition* part)
{
    return -1;
}
