//
//  statedata.c
//  MorPhy2
//
//  Created by mbrazeau on 26/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "mplerror.h"
#include "morphy.h"
#include "statedata.h"

char* mpl_skip_nexus_comment(const char *c)
{
    if (*c != '[') {
        return (char*)ERR_BAD_PARAM;
    }
    char *ret = (char*)c;
    
    do {
        ++ret;
    } while (*ret != ']');
    
    return ret;
}

int compare_char_t_states(const void *ptr1, const void *ptr2)
{
    return *(char*)ptr1 - *(char*)ptr2;
}

int mpl_get_states_from_rawdata(Morphyp handl)
{
    
    assert(handl);
    
    int count = 0;
    char *rawmatrix = handl->char_t_matrix;
    char *current = NULL;
    int listmax = MAXSTATES + 1; // +1 for terminal null.
    char statesymbols[listmax];
    int dbg_loopcount = 0;
    
    statesymbols[0] = '\0';
    current = rawmatrix;
    
    do {
        if (strchr(gmpl_valid_symb, *current)) {
            if (*current == '[') {
                current = mpl_skip_nexus_comment(current);
            }
            if (!strchr(statesymbols, *current) &&
                strchr(gmpl_valid_state, *current)) {
                // Put in list
                statesymbols[count] = *current;
                ++count;
                statesymbols[count] = '\0';
            }
        }
        else {
            return ERR_INVALID_SYMBOL;
        }
        
        ++current;
        ++dbg_loopcount;
    } while (*current);
    
    // Sort alphanumerically
    qsort(statesymbols, strlen(statesymbols), sizeof(char),
          compare_char_t_states);
    
    return count-1;
}

void mpl_set_numsymbols(int numstates, Morphyp handl)
{
    assert(!numstates && !handl);
    
    handl->numsymbols = numstates;
}

int mpl_get_numsybols(Morphyp handl)
{
    assert(handl);
    return handl->numsymbols;
}

int mpl_create_state_dictionary(const char* states, Morphyp handl)
{
    assert(!strchr(states, ' '));
    
    mpl_set_numsymbols((int)strlen(states), handl);
    
    
    return ERR_NO_ERROR;
}


void mpl_convert_rawdata(Morphyp handl)
{
    
    if (!handl->symbols) {
        mpl_get_states_from_rawdata(handl);
    }
    else {
        // TODO: Check all states are valid values
        // TODO: Replace list to one without spaces? (YES)
    }
    
    // Create a state dictionary
    // Set shift values in the dictionary
    // Use dictionary to convert
}

