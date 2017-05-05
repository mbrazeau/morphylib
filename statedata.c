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

char* mpl_skip_closure(const char *closure, const char openc, const char closec)
{
    if (*closure != openc) {
        return (char*)ERR_BAD_PARAM;
    }
    char *ret = (char*)closure;
    
    do {
        ++ret;
        if (*ret == closec) {
            return ret;
        }
    } while (*ret);
    
    return NULL;
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
    
//    dbg_printf("The raw matrix about to be processed:\n%s\n", rawmatrix);
    
    do {
//        dbg_printf("Current: %c\n", *current);
        
        if (strchr(gmpl_valid_symb, *current)) {
            
            if (strchr(gmpl_valid_matrix_punc, *current)) {
                ++current;
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
//            dbg_printf("Returning error\n");
            return ERR_INVALID_SYMBOL;
        }
        
        ++current;
        ++dbg_loopcount;
    } while (*current);
    
    // Sort alphanumerically
    qsort(statesymbols, strlen(statesymbols), sizeof(char),
          compare_char_t_states);
    
    dbg_printf("The state symbols: %s\n", statesymbols);
    
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

bool mpl_is_valid_matrix_symbol(const char c)
{
    if (strchr(gmpl_valid_state, c)) {
        return true;
    }
    else if (c == '?') {
        return true;
    }
    else if (c == '(' || c == ')') {
        return true;
    }
    
    return false;
}

unsigned long mpl_get_valid_matrix_length(const char* rawmatrix)
{
    unsigned long len = 0;
    char* matptr = (char*)rawmatrix;
    
    do {
        if (mpl_is_valid_matrix_symbol(*matptr)) {
            ++len;
//        }
//        else if (*matptr == '?') {
//            ++len;
//        }
//        else if (*matptr == '(' || *matptr == ')') {
//            ++len;
        }
        else if (*matptr == '[') {
            matptr = mpl_skip_closure(matptr, '[', ']');
            assert(!(matptr < 0));
        }
        ++matptr;
    } while (*matptr);
    
    return len;
}

void mpl_copy_valid_matrix_data(char *copy, const char* rawmatrix)
{
    int i = 0;
    char* matptr = (char*)rawmatrix;
    
    do {
        if (mpl_is_valid_matrix_symbol(*matptr)) {
            copy[i] = *matptr;
            ++i;
        }
        else if (*matptr == '[') {
            matptr = mpl_skip_closure(matptr, '[', ']');
            assert(!(matptr < 0));
        }
        ++matptr;
    } while (*matptr);
    
    copy[i] = '\0';
    dbg_printf("The truncated matrix: %s\n", copy);
}

// Copy the raw matrix, take out whitespace and comments
int mpl_copy_raw_matrix(const char* rawmatrix, Morphyp handl)
{
    unsigned long len = mpl_get_valid_matrix_length(rawmatrix);
    
    char *matcpy = (char*)calloc(len + 1, sizeof(char));
    if (!matcpy) {
        return ERR_BAD_MALLOC;
    }
    mpl_copy_valid_matrix_data(matcpy, rawmatrix);
    handl->char_t_matrix = matcpy;
    return ERR_NO_ERROR;
}

int mpl_check_nexus_matrix_dimensions
(char *preproc_matrix, int input_num_taxa, int input_num_chars)
{
    /* An input matrix should not have inline taxon names. This function
     * scans each row of the input matrix to determine whether or not the
     * number of places in the row corresponds to input number of
     * of characters. If the number exceeds the expected number of data
     * columns (num_input_chars), then it is inferred that taxon names or
     * other extraneous info are included in the matrix. */
    
    char* current = NULL;
    int matrix_size = 0;
    int expected_size = 0;
    
    expected_size = input_num_chars * input_num_taxa;
    
    current = preproc_matrix;
    
    do {
        if (strchr(gmpl_valid_state, *current) || *current == '?') {
            ++matrix_size;
        }
        else if (*current == '(' || *current == '{') {
            current = mpl_skip_closure(current, '(', ')');
            if (current < 0) {
                mpl_skip_closure(current, '{', '}');
            }
            assert(!(current < 0));
            ++matrix_size;
        }

        ++current;
    } while (*current/* != ';'*/);
    
    if (matrix_size > expected_size) {
        return ERR_DIMENS_UNDER;
    }
    else if (matrix_size < expected_size) {
        return ERR_DIMENS_OVER;
    }
    
    return ERR_NO_ERROR;
}


char* mpl_get_preprocessed_matrix(Morphyp handl)
{
    assert(handl);
    return handl->char_t_matrix;
}

Mstates mpl_gap_value(Morphyp handl)
{
    switch (mpl_get_gaphandl(handl)) {
        case GAP_INAPPLIC:
            return NA;
        case GAP_MISSING:
            return MISSING;
        case GAP_NEWSTATE:
            return (Mstates)1;
        case GAP_MAX:
            return -1;
        default:
            break;
    }
    
    return -2;
}

MPLmatrix* mpl_new_mpl_matrix
(const int ntaxa, const int nchar, const int nstates)
{
    MPLmatrix* ret = NULL;
    
    ret = (MPLmatrix*)calloc(1, sizeof(MPLmatrix));
    if (!ret) {
        return NULL;
    }
    
    
    ret->intweights = (int*)calloc(nchar, sizeof(int));
    if (!ret->intweights) {
        mpl_delete_mpl_matrix(ret);
        return NULL;
    }
    
    ret->fltweights = (Mflt*)calloc(nchar, sizeof(Mflt));
    if (!ret->fltweights) {
        mpl_delete_mpl_matrix(ret);
        return NULL;
    }
    
    ret->cells = (MPLcell*)calloc(ntaxa * nchar, sizeof(MPLcell));
    if (!ret->cells) {
        mpl_delete_mpl_matrix(ret);
        return NULL;
    }
    
    ret->ncells = ntaxa * nchar;
    
    for (int i = 0; i < ret->ncells; ++i) {
        ret->cells[i].asstr = (char*)calloc(nstates + 1, sizeof(char));
        if (!ret->cells[i].asstr) {
            for (int j = 0; j < i; ++j) {
                free(ret->cells[i].asstr);
                ret->cells[i].asstr = NULL;
            }
            mpl_delete_mpl_matrix(ret);
            return NULL;
        }
    }
    
    
    return ret;
}

int mpl_delete_mpl_matrix(MPLmatrix* m)
{
    assert(m);
    if (m->cells) {
        for (int i = 0; i < m->ncells; ++i) {
            if (m->cells[i].asstr) {
                free(m->cells[i].asstr);
                m->cells[i].asstr = NULL;
            }
        }
        free(m->cells);
    }
    if (m->fltweights) {
        free(m->fltweights);
        m->fltweights = NULL;
    }
    if (m->intweights) {
        free(m->intweights);
        m->intweights = NULL;
    }
    
    return ERR_NO_ERROR;
}

void mpl_convert_rawdata(Morphyp handl)
{
    
    if (!handl->symboldict->rawsymbols) {
        
        mpl_get_states_from_rawdata(handl);
    }
    else {
        dbg_printf("Skipping attempt to make state list\n");
        // TODO: Check all states are valid values
        // TODO: Replace list to one without spaces? (YES)
    }
    
    
    // Loop over the matrix and find out which ones have
    // Create a state dictionary
    // Probably multipe dictionaries...? Regular and w/ NA?
    mpl_create_state_dictionary(handl);
    // Set shift values in the dictionary
    // Use dictionary to convert
}

