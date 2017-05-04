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
    
    unsigned long numstates = strlen(statesymbols);
    handl->symboldict->numstates = (int)numstates;
    handl->symboldict->rawsymbols = (char*)malloc((1 + numstates)
                                                  * sizeof(char));
    strcpy(handl->symboldict->rawsymbols, statesymbols);
    
    return count-1;
}

void mpl_set_numsymbols(int numstates, Morphyp handl)
{
    dbg_printf("Setting numsymbols\n");
    assert(numstates && handl);
    handl->symboldict->numstates = numstates;
}

int mpl_get_numsybols(Morphyp handl)
{
    dbg_printf("Getting numsymbols\n");
    assert(handl);
    return handl->symboldict->numstates;
}

int mpl_create_state_dictionary(Morphyp handl)
{
    dbg_printf("Creating state dictionary\n");
    assert(handl->symboldict);
    
    char* states = handl->symboldict->rawsymbols;
    assert(!strchr(states, ' '));
    
    mpl_set_numsymbols((int)strlen(states), handl);
    int maxsymb = mpl_get_numsybols(handl);
    
    handl->symboldict->symbols = (MPL_stsymb*)calloc(maxsymb,
                                                     sizeof(MPL_stsymb));
    if (!handl->symboldict->symbols) {
        return ERR_BAD_MALLOC;
    }
    
    
    for (int i = 0; i < maxsymb; ++i) {
        handl->symboldict->symbols[i].aschar = states[i];
        dbg_printf("Converting symbol: %c\n", states[i]);
    }
    
    return ERR_NO_ERROR;
}


MPL_symbset* mpl_alloc_symbolset(void)
{
    return (MPL_symbset*)calloc(1, sizeof(MPL_symbset));
}

void mpl_destroy_symbolset(MPL_symbset* symbs)
{
    if (symbs) {
        if (symbs->rawsymbols) {
            free(symbs->rawsymbols);
            symbs->rawsymbols = NULL;
        }
        if (symbs->symbols) {
            free(symbs->rawsymbols);
        }
        
        free(symbs);
    }
}

void mpl_convert_rawdata(Morphyp handl)
{
    
    if (!handl->symboldict->rawsymbols) {
        
        mpl_get_states_from_rawdata(handl);
    }
    else {
        // TODO: Check all states are valid values
        // TODO: Replace list to one without spaces? (YES)
    }
    
    // Create a state dictionary
    // Probably multipe dictionaries...? Regular and w/ NA?
    mpl_create_state_dictionary(handl);
    // Set shift values in the dictionary
    // Use dictionary to convert
}

