//
//  fitch.h
//  MorPhy2
//
//  Created by mbrazeau on 02/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef fitch_h
#define fitch_h

int mpl_fitch_downpass
(MPLstate* left, MPLstate* right, MPLstate* n, int nchars);

int mpl_fitch_uppass
(MPLstate *left, MPLstate *right, MPLstate *n, MPLstate *anc, int nchars);

int mpl_NA_fitch_first_downpass
(MPLstate* left, MPLstate *right, MPLstate *n, int nchars);

int mpl_NA_fitch_first_uppass
(MPLstate *left, MPLstate *right, MPLstate *n, MPLstate *anc, int nchars);

int mpl_NA_fitch_second_downpass
(MPLstate* left, MPLstate* right, MPLstate* n, int nchars);

int mpl_NA_fitch_second_uppass
(MPLstate *left, MPLstate *right, MPLstate *n, MPLstate *anc, int nchars);
#endif /* fitch_h */
