//
//  mplerror.h
//  MorPhy2
//
//  Created by mbrazeau on 04/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef mplerror_h
#define mplerror_h

typedef enum {
    
    ERR_NO_DATA,
    ERR_BAD_MALLOC,
    ERR_BAD_PARAM,
    ERR_UNEXP_NULLPTR,
    ERR_INVALID_SYMBOL,
    ERR_NO_ERROR            = 0, // All errors are negative ints
} MPL_ERR_T;

#endif /* mplerror_h */
