//
//  morphy.c
//  MorPhy2
//
//  Created by mbrazeau on 23/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mplerror.h"
#include "morphy.h"
#include "statedata.h"

void *mpl_alloc(size_t size, int setval)
{
    void *ret = malloc(size);
    if (ret) {
        memset(ret, setval, size);
    }
    return ret;
}

Morphyp mpl_new_Morphy_t(void)
{
    return (Morphyp)calloc(1, sizeof(Morphy_t));
}

void* mpl_get_from_matrix
(const int row, const int col, const size_t size, const void* data)
{
    return (void*)&data[row * col * size + (size * col - 1)];
}
