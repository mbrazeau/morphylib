//
//  fitch.h
//  MorPhy2
//
//  Created by mbrazeau on 02/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef fitch_h
#define fitch_h

int mpl_fitch_downpass(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLpartition* part);

int mpl_fitch_uppass(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLstatesets* anc, MPLpartition* part);

int mpl_NA_fitch_first_downpass(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLpartition* part);

int mpl_NA_fitch_first_uppass(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLstatesets* anc, MPLpartition* part);

int mpl_NA_fitch_second_downpass(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLpartition* part);

int mpl_NA_fitch_second_uppass(MPLstatesets* lset, MPLstatesets* rset, MPLstatesets* nset, MPLstatesets* anc, MPLpartition* part);

#endif /* fitch_h */
