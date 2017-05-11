//
//  statedata.c
//  MorPhy2
//
//  Created by mbrazeau on 26/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "morphydefs.h"
#include "mplerror.h"
#include "morphy.h"
#include "statedata.h"
#include "mpl.h"

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


int mpl_compare_symbol_lists(const char* sym1, const char* sym2)
{
    int i = 0;
    
    for (i = 0; sym1[i]; ++i) {
        if (!strchr(sym2, sym1[i])) {
            if (!isspace(sym1[i])) {
                return 1;
            }
        }
    }
    
    for (i = 0; sym2[i]; ++i) {
        if (!strchr(sym1, sym2[i])) {
            if (!isspace(sym2[i])) {
                return 1;
            }
        }
    }
    
    return 0;
}


int mpl_assign_symbol_list_from_matrix
(const char *symbs, struct MPLsymbols* symlist)
{
    assert(symbs && symlist);
    
    int nsymbs = (int)strlen(symbs);
    
    ++nsymbs;
    symlist->symbolsinmatrix = (char*)calloc(nsymbs, sizeof(char));
    
    if (!symlist->symbolsinmatrix) {
        return ERR_BAD_MALLOC;
    }
    
    strcpy(symlist->symbolsinmatrix, symbs);
    
    return ERR_NO_ERROR;
}

char *mpl_query_symbols_from_matrix(Morphyp m)
{
    return m->symbols.symbolsinmatrix;
}


int mpl_get_states_from_rawdata(Morphyp handl)
{
    
    assert(handl);
    
    int count = 0;
    char *rawmatrix = handl->char_t_matrix;
    char *current = NULL;
    int listmax = MAXSTATES + 1; // +1 for terminal null.
    char statesymbols[listmax];
//    int dbg_loopcount = 0;
    
    statesymbols[0] = '\0';
    current = rawmatrix;
    
    do {
        if (strchr(VALIDSYMB, *current)) {
            
            if (strchr(VALID_NEXMAT_PUNC, *current)) {
                ++current;
            }
            if (!strchr(statesymbols, *current) &&
                strchr(VALID_STATESYMB, *current)) {
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
    
    } while (*current);
    
    // Sort alphanumerically
    qsort(statesymbols, strlen(statesymbols), sizeof(char),
          compare_char_t_states);
    
    dbg_printf("The state symbols: %s\n", statesymbols);
    int numstates = (int)strlen(statesymbols);
    mpl_set_numsymbols(numstates, handl);
    mpl_assign_symbol_list_from_matrix(statesymbols, &handl->symbols);
    return count-1;
}


int mpl_set_numsymbols(int numsymb, Morphyp handl)
{
    dbg_printf("Setting numsymbols\n");
    assert(handl);
    handl->symbols.numstates = numsymb;
    return ERR_NO_ERROR;
}

int mpl_get_numsymbols(Morphyp handl)
{
    dbg_printf("Getting numsymbols\n");
    assert(handl);
    return handl->symbols.numstates;
}


int mpl_create_state_dictionary(Morphyp handl)
{
    int i           = 0;
    int gappush     = 0;
    int numsymbs    = handl->symbols.numstates;
    char *symbols   = mpl_get_symbols((Morphy)handl);
    assert(symbols);
    
    if (!handl->symbols.packed) {
        
        handl->symbols.packed = (MPLstate*)calloc(handl->symbols.numstates,
                                                  sizeof(MPLstate));
        if (!handl->symbols.packed) {
            return ERR_BAD_MALLOC;
        }
    }

    if (handl->gaphandl == GAP_INAPPLIC || handl->gaphandl == GAP_NEWSTATE) {
        gappush = 1;
    }
    
    for (i = 0; i < numsymbs; ++i) {
        handl->symbols.packed[i] = 1 << (i + gappush);
    }
    
    return ERR_NO_ERROR;
}


MPLstate mpl_convert_gap_symbol(Morphyp handl)
{
    if (handl->gaphandl == GAP_INAPPLIC) {
        return NA;
    }
    else if (handl->gaphandl == GAP_NEWSTATE) {
        return (MPLstate)1;
    }
    else if (handl->gaphandl == GAP_MISSING) {
        return MISSING;
    }
    
    return ERR_NO_DATA;
}

MPLstate mpl_convert_char_to_MPLstate(const char* celldata, Morphyp handl)
{
    int i = 0;
    MPLstate result = 0;
    
    do {
        i = 0;
        do {
            if (*celldata == handl->symbols.statesymbols[i]) {
                result |= handl->symbols.packed[i];
            }
            ++i;
        } while (handl->symbols.statesymbols[i]);
        ++celldata;
    } while (*celldata);
    
    return result;
}

int mpl_convert_cells(Morphyp handl)
{
    
    int i = 0;
    int numcells = handl->inmatrix.ncells;
    MPLmatrix *inmatrix = &handl->inmatrix;
    char *celldata = NULL;
    
    for (i = 0; i < numcells; ++i) {
        celldata = inmatrix->cells[i].asstr;
        if (*celldata == handl->symbols.gap) {
            inmatrix->cells[i].asint = mpl_convert_gap_symbol(handl);
        }
        else if (*celldata == handl->symbols.missing) {
            inmatrix->cells[i].asint = MISSING;
        }
        else {
            inmatrix->cells[i].asint = mpl_convert_char_to_MPLstate(celldata,
                                                                    handl);
        }
    }
    
    return ERR_NO_ERROR;
}

//int mpl_init_symbolset(Morphyp m)
//{
//    if (!mpl_get_numsymbols(m)) {
////        //somthing
//    }
//    return ERR_NO_ERROR;
//}


void mpl_destroy_symbolset(Morphyp m)
{
    assert(m);
    if (m->symbols.statesymbols) {
        free(m->symbols.statesymbols);
        m->symbols.statesymbols = NULL;
    }
}


bool mpl_is_valid_matrix_symbol(const char c)
{
    if (strchr(VALID_STATESYMB, c)) {
        return true;
    }
    else if (strchr(VALID_WILDCAR, c)) {
        return true;
    }
    else if (strchr(VALID_NEXMAT_PUNC, c)) {
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
    
    copy[i-1] = '\0';
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
    assert(current);
    
    do {
        if (strchr(VALID_STATESYMB, *current)
            || strchr(VALID_WILDCAR, *current)) {
            ++matrix_size;
        }
        else if (*current == '(' || *current == '{') {
            
            char* err = 0;
            
            if (*current == '(') {
                err = mpl_skip_closure(current, '(', ')');
            }
            else {
                err = mpl_skip_closure(current, '{', '}');
            }
            if (err <= 0) {
                return ERR_MATCHING_PARENTHS;
            }
            
            current = err;
            assert(!(current < 0));
            ++matrix_size;
        }

        ++current;
    } while (*current);
    
    dbg_printf("Expected dimensions: %i\n", expected_size);
    dbg_printf("matrix length: %i\n", matrix_size);
    
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


MPLstate mpl_gap_value(Morphyp handl)
{
    switch (mpl_get_gaphandl(handl)) {
        case GAP_INAPPLIC:
            return NA;
        case GAP_MISSING:
            return MISSING;
        case GAP_NEWSTATE:
            return (MPLstate)1;
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
    assert(nstates);
    MPLmatrix* ret = NULL;
    
    ret = (MPLmatrix*)calloc(1, sizeof(MPLmatrix));
    if (!ret) {
        return NULL;
    }
    
    ret->chtypes = (MPLchtype*)calloc(nchar, sizeof(MPLchtype));
    if (!ret->chtypes) {
        mpl_delete_mpl_matrix(ret);
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
    int i = 0;
    
    for (i = 0; i < ret->ncells; ++i) {
        ret->cells[i].asstr = (char*)calloc(nstates + 1, sizeof(char));
        if (!ret->cells[i].asstr) {
            int j = 0;
            for (j = 0; j < i; ++j) {
                free(ret->cells[i].asstr);
                ret->cells[i].asstr = NULL;
            }
            mpl_delete_mpl_matrix(ret);
            return NULL;
        }
    }
    
    return ret;
}

int mpl_init_inmatrix(Morphyp handl)
{
    assert(handl);
    MPLmatrix* mat = &handl->inmatrix;
    int ntaxa = mpl_get_numtaxa((Morphyp)handl);
    int nchar = mpl_get_num_charac((Morphyp)handl);
    int nstates = mpl_get_numsymbols(handl);
    
    mat->chtypes = (MPLchtype*)calloc(nchar, sizeof(MPLchtype));
    if (!mat->chtypes) {
        return ERR_BAD_MALLOC;
    }
    
    mat->intweights = (int*)calloc(nchar, sizeof(int));
    if (!mat->intweights) {
        mpl_delete_mpl_matrix(mat);
        return ERR_BAD_MALLOC;
    }
    
    mat->fltweights = (Mflt*)calloc(nchar, sizeof(Mflt));
    if (!mat->fltweights) {
        mpl_delete_mpl_matrix(mat);
        return ERR_BAD_MALLOC;
    }
    
    mat->cells = (MPLcell*)calloc(ntaxa * nchar, sizeof(MPLcell));
    if (!mat->cells) {
        mpl_delete_mpl_matrix(mat);
        return ERR_BAD_MALLOC;
    }
    
    mat->ncells = ntaxa * nchar;
    int i = 0;
    
    for (i = 0; i < mat->ncells; ++i) {
        mat->cells[i].asstr = (char*)calloc(nstates + 1, sizeof(char));
        if (!mat->cells[i].asstr) {
            int j = 0;
            for (j = 0; j < i; ++j) {
                free(mat->cells[i].asstr);
                mat->cells[i].asstr = NULL;
            }
            mpl_delete_mpl_matrix(mat);
            return ERR_BAD_MALLOC;
        }
    }
    
    return ERR_NO_ERROR;
}

int mpl_delete_mpl_matrix(MPLmatrix* m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    int i = 0;
    
    if (m->cells) {
        for (i = 0; i < m->ncells; ++i) {
            if (m->cells[i].asstr) {
                free(m->cells[i].asstr);
                m->cells[i].asstr = NULL;
            }
        }
        free(m->cells);
    }
    
    if (m->chtypes) {
        free(m->chtypes);
        m->chtypes = NULL;
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


MPLmatrix* mpl_get_mpl_matrix(Morphyp m)
{
    return &m->inmatrix;
}


int mpl_set_gap_push(Morphyp handl)
{
    gap_t gt = mpl_get_gaphandl(handl);
    
    if (gt == GAP_INAPPLIC || gt == GAP_NEWSTATE) {
        return 1;
    }
    else if (gt == GAP_MISSING) {
        return 0;
    }
    
    return -1;
}


int mpl_get_uncorrected_shift_value(char symb, Morphyp handl)
{
    // Gets the raw shift value as determined by the order in the symbols list
    assert(symb != DEFAULTGAP && symb != DEFAULTMISSING);
    int shift = 0;
    char* symbols = mpl_get_symbols((Morphy)handl);
    
    while (*symbols != symb && *symbols) {
        ++symbols;
        ++shift;
    }
    
    return shift;
}


void mpl_use_symbols_from_matrix(Morphyp handl)
{
    handl->symbols.statesymbols = handl->symbols.symbolsinmatrix;
}


int mpl_write_input_rawchars_to_cells(Morphyp handl)
{
    int i = 0;
    int j = 0;
    int rows = mpl_get_numtaxa((Morphyp)handl);
    int cols = mpl_get_num_charac((Morphyp)handl);
    int length = rows * cols;
    
    char* prpdata = mpl_get_preprocessed_matrix(handl);
    
    dbg_printf("Made it this far...\n%s\n", prpdata);
    
    while (*prpdata) {
        
        if (!strchr(VALID_NEXMAT_PUNC, *prpdata)) {
            handl->inmatrix.cells[i].asstr[0] = *prpdata;
            handl->inmatrix.cells[i].asstr[1] = '\0';
        }
        else {
            if (*prpdata == '(') {
                j = 0;
                ++prpdata;
                do {
                    
                    handl->inmatrix.cells[i].asstr[j] = *prpdata;
                    ++j;
                    ++prpdata;
                } while (*prpdata != ')');
                handl->inmatrix.cells[i].asstr[j] = '\0';
            }
            
            if (*prpdata == '{') {
                j = 0;
                ++prpdata;
                do {
                    handl->inmatrix.cells[i].asstr[j] = *prpdata;
                    ++j;
                    ++prpdata;
                } while (*prpdata != '}');
                handl->inmatrix.cells[i].asstr[j] = '\0';
            }
            if (*prpdata == ';') {
                break;
            }
        }

        ++i;
        ++prpdata;
    };
    
    prpdata = mpl_get_preprocessed_matrix(handl);
    dbg_printf("Now: %s\n", prpdata);

    for (i = 0; i < length; ++i) {
        dbg_printf("%s ", handl->inmatrix.cells[i].asstr);
    }
    dbg_printf("\n");
    
    return ERR_NO_ERROR;
}

int mpl_update_partitions(MPLmatrix mat)
{
    // TODO: Implement
    
    // Note: maybe create a stack of free partitions that can be listed and then
    // taken down each time the partitions list is updated. This will keep the
    // analysis state of the matrix 'ready to go' at any time.
    
    return ERR_NO_ERROR;
}




int mpl_convert_rawdata(Morphyp handl)
{
    int ret = ERR_NO_ERROR;
    
    mpl_get_states_from_rawdata(handl);
    
    // If no symbols supplied by the user
    if (!mpl_get_symbols((Morphy)handl)) {
        // Assign internal symbols to list
        mpl_use_symbols_from_matrix(handl);
    }
    else {
        
        char *frommatrix    = mpl_query_symbols_from_matrix(handl);
        char *user          = mpl_get_symbols((Morphyp)handl);
        
        if (mpl_compare_symbol_lists(frommatrix, user)) {
            return ERR_SYMBOL_MISMATCH;
        }
    }
    
    mpl_init_inmatrix(handl);
    
//    handl->inmatrix = mpl_new_mpl_matrix(handl->numtaxa,
//                                         handl->numcharacters,
//                                         mpl_get_numsymbols(handl));
//    if (!handl->inmatrix) {
//        return ERR_BAD_MALLOC;
//    }
    
    // Now safe to write characters into cells.
    mpl_write_input_rawchars_to_cells(handl);
    mpl_create_state_dictionary(handl);

    // Use dictionary to convert
    mpl_convert_cells(handl);
    
    return ret;
}

