#include "tldefs.h"
#include "treelib.h"
#include "tlerror.h"
#include "tltree.h"
#include "tlutils.h"
#include "tlnewick.h"

#include <stdio.h>


char *tl_read_Newick_brlen(TLflt *brlen, const char* str)
{
	char brlenstr[100];
    char *c = (char*)str;
	int i = 0;
    
    do {
        brlenstr[i++] = *c;
        ++c;
	} while (isalnum(brlenstr[i-1]) || brlenstr[i-1]  == '.');
    
    *brlen = (TLflt)atof(brlenstr);
    --c;
	return c;
}


char *tl_read_token(char* token, const char* str)
{
    char *c = (char*)str;
    int i = 0;
    
    do {
        token[i] = *c;
        if (token[i] == '_') {
            token[i] = ' ';
        }
        ++i;
        ++c;
    } while (isalnum(token[i-1]) || strchr(NWKTKNSYMB, token[i-1])/*token[i-1]  == ' '|| token[i-1]  == '_' || token[i-1]  == '\''*/);
    
    token[i-1] = '\0';
    --c;
    return c;
}


char* tl_tokenise_Newick(const char *nwk)
{
    char *tknNwk = tl_strdup(nwk);
    strtok(tknNwk, NWKDELIM);
    return tknNwk;
}


int tl_free_tokenised_Newick(char *nwktknd)
{
    if (nwktknd) {
        free(nwktknd);
        return ERR_NOERR;
    }
    return ERR_BAD_FREE;
}


char *tl_extract_taxlabel(char *token)
{
    int i = 0;
    char* label = NULL;
    
    if (*token == ' ' || *token == '(') {
        do {
            ++token;
        } while (*token == ' ' || *token == '(');
    }
    
    if (*token != ':' && *token != ')') {
        unsigned long tklen = strlen(token);
        ++tklen;
        label = (char*)tl_alloc(tklen * sizeof(char), 0);
        int endtok;

        do {
            endtok = 0;
            if (isalnum(*token) || *token==' ' || *token=='\'' ||
                *token=='_' || *token == '"')
            {
                if (*token != '\'') {
                    label[i] = *token;
                    if (label[i] == '_') {
                        label[i] = ' ';
                    }
                    ++i;
                }
                ++token;
                endtok = 1;
            }
        } while (endtok);
    }
    return label;
}


int tl_recurse_Newick(char **nwk, int* index, int *polist, TLhandlp handl)
{
    //dbg_printf("Recursing Newick\n");
    //dbg_printf("Newick: %s\n", *nwk);
//    int innode = index;
    TLflt brlen;
    int descindex = 0;
    int nindex = *index;
    
    assert(nindex);
    assert(nindex >= handl->dimensions.ntaxa);
    
    // TODO: This (and related operations) need basic safety.
    //       Any overflow of max token size needs to be made up
    //       in order to prevent a crash.
    char label[MAX_TOKEN_SIZE];
    
    ++(*nwk);
    
    do {
        if (**nwk == '(') {
            --(*index);
            descindex = tl_recurse_Newick(nwk, index, polist, handl);
            // Point store current node's index in that label's cell
            //dbg_printf("Descindex: %i\n", descindex);
            polist[descindex] = nindex;
        }

        if (**nwk == ':') {
            ++(*nwk);
            tl_read_Newick_brlen(&brlen, *nwk);
            // TODO: remove dbg_printf call
            //dbg_printf("Branch length: %f\n", brlen);
        }

        if (isalnum(**nwk) || **nwk == '\'') {
            *nwk = tl_read_token(label, *nwk);
            
            
            // TODO: remove dbg_printf calls
            //dbg_printf("Label: %s\n", label);
            if ((descindex = atoi(label))) {
                // Lookup the descindex;
                //dbg_printf("Is a number!\n");
                polist[descindex-1] = nindex;
                descindex = 0;
            }
            else {
                //dbg_printf("Not a number!\n");
                // Get label num from taxlist
                descindex = tl_lookup_label(0, label, handl);
                polist[descindex] = nindex;
                //dbg_printf("Res: %i\n", descindex);
                descindex = 0;
            }
            
            // Point store current node's index in that label's cell
        }
        if (**nwk == ',') {
            ++(*nwk);
        }
        if (**nwk == ' ') {
            ++(*nwk);
        }

    } while (**nwk != ')');
    
    ++(*nwk);
    
    return nindex;
}


int tl_verify_Newick(const char *nwk)
{
    int i = 0;
    bool spaceOK = false;
    unsigned long nwklen = strlen(nwk);
    int open = 0, close = 0, comma = 0, badspace = 0;
    
    int end = strlen(nwk);
    if (nwk[end-1] != ';') {
        // fprintf(stderr, "Shiiiiit\n");
        // TODO: Check and test this
        //    dbg_printf("Bad nwk!: %c\n", nwk[nwklen-1]);
        return ERR_NWK_END;
    }
    
    while (nwk[i]) {
        
        if (nwk[i] == '\'') {
            if (spaceOK == false) {
                spaceOK = true;
            }
            else {
                spaceOK = false;
            }
        }
        
        if (nwk[i] == ' ') {
            if (spaceOK == false) {
                ++badspace;
            }
        }
        
        if (nwk[i] == '(') {
            ++open;
        }
        else if (nwk[i] == ')') {
            ++close;
        }
        else if (nwk[i] == ',') {
            ++comma;
        }
        ++i;
    }
    
    if (badspace) {
        dbg_printf("Error: embedded spaces in Newick string\n");
        return ERR_SYMBOL;
    }
    
    if (open > close) {
        dbg_printf("Error: missing closing bracket\n");
        return ERR_OPARENS_EXC;
    }
    else if (close > open) {
        dbg_printf("Error: missing opening bracket\n");
        return ERR_CPARENS_EXC;
    }
    
    return ERR_NOERR;
}


int tl_compr_by_index(const void* ptr1, const void *ptr2)
{
    TLlabel *lab1 = *(TLlabel**)ptr1;
    TLlabel *lab2 = *(TLlabel**)ptr2;
    return lab1->nodeindex - lab2->nodeindex;
}


int tl_check_Newick_labels(const char *nwk, TLhandlp handl)
{
    // Tokenise the string.
    char *nwktknd = tl_tokenise_Newick(nwk);

    // Check that a list of labels exists
    // If: labels
    if (handl->taxlabels.labels) {
        int taxacount = 0;
        char *tkn = NULL;
        
        tkn = tl_extract_taxlabel(nwktknd);
        
        if (tl_check_duplicate_label(tkn, &handl->taxlabels)) {
            free(tkn);
            free(nwktknd);
            return ERR_BAD_PARAM;

        }
        free(tkn);
        tkn = NULL;
        ++taxacount;
        
        // Check all tokens are found in list
        while ((tkn = strtok(NULL, NWKDELIM))) {
            
            tkn = tl_extract_taxlabel(tkn);
            
            if (tl_check_duplicate_label(tkn, &handl->taxlabels)) {
                free(tkn);
                free(nwktknd);
                return ERR_BAD_PARAM;
            }
            free(tkn);
            tkn = NULL;
            ++taxacount;
        }
        
        if (taxacount != tl_get_numtaxa(handl)) {
            free(tkn);
            tl_free_tokenised_Newick(nwktknd);
            return ERR_BAD_PARAM;
        }
    }
    else {
        // Extract the taxon labels and push them to the list.
        char *tkn = NULL;// strtok(nwktknd, NwkDelim);
        
        tkn = tl_extract_taxlabel(nwktknd);
        tl_push_new_label(tkn, &handl->taxlabels);
        // TODO: FIX THIS:
        //++handl->dimensions.ntaxa;
        free(tkn);
        //        tkn = NULL;
        
        while ((tkn = strtok(NULL, NWKDELIM))) {
            tkn = tl_extract_taxlabel(tkn);
            tl_push_new_label(tkn, &handl->taxlabels);
//            ++handl->dimensions.ntaxa;
            free(tkn);
            tkn = NULL;
        }
        
        // Sort?
        qsort(handl->taxlabels.labels,
              handl->taxlabels.nelems,
              sizeof(TLlabel*),
              tl_compr_by_index);
    }
    
    return tl_free_tokenised_Newick(nwktknd);
}

int tl_is_rooted(const char* nwk)
{
    char *rootcmd = NULL;
    
    rootcmd = strstr(nwk, NWKROOTCMD);
    
    if (rootcmd) {
        
        rootcmd = rootcmd + 2;
        
        if (toupper(*rootcmd) == 'R') {
            return 1;
        }
        else if (toupper(*rootcmd) == 'U') {
            return 0;
        }
    }
    
    return -1;
}

char* tl_search_nwk_rootcmd(const char *nwk)
{
    return strstr(nwk, NWKROOTCMD);
}

int tl_parse_nwk_rooting(const char* nwk)
{
    char *rootcmd = tl_search_nwk_rootcmd(nwk);
    if (tl_is_rooted(rootcmd)) {
        return 1;
    }
    
    return 0;
}


int tl_read_Newick(const char *nwk, TLhandlp handl)
{
    int ret = ERR_NOERR;
    char *nwkptr = (char*)nwk;
    // TODO: Verify newick validity
    if ((ret = tl_verify_Newick(nwk))) {
        return ret;
    }
    
    if ((ret = tl_check_Newick_labels(nwk, handl))) {
        return ret;
    }
    
    // Check maxtrees (and autoinc default)
    // If there's room:
    //      Create new topology for Newick input
    int numtaxa = tl_get_numtaxa(handl);
    TLtopol *new = tl_create_topology_rec(numtaxa);
    dbg_printf("numtaxa: %i\n", numtaxa);
    
    if (tl_is_rooted(nwk)) {
        new->rooted = true;
    }
    else {
        new->rooted = false;
    }
    
    int startnode = (2 * numtaxa - 2);
    
    int root = tl_recurse_Newick(&nwkptr, &startnode, new->edgeIDs, handl);
    
    // TODO: This assumes edge[numtaxa] is the root, but maybe make root last
    //new->edgeIDs[numtaxa] = new->nnodes;
    
    new->edgeIDs[new->nnodes-1] = new->nnodes;
    
#ifdef DEBUG
    int i = 0;
    
    for (i = 0; i < new->nnodes; ++i) {
        dbg_printf("%i ",i);
    }
    dbg_printf("\n");
    for (i = 0; i < new->nnodes; ++i) {
        dbg_printf("%i ", new->edgeIDs[i]);
    }
    dbg_printf("\n");
    // Clean up resources; return success
#endif
    tl_push_topol(new, &handl->treelist);
    return ERR_NOERR;
}


void tl_write_Newick_traversal
(TLnode *n, bool dolabels, char** result, TLstrlist* labels)
{
    if (n->tip) {
        if (dolabels) {
            // You'll need to fetch the labels
        } else {
            char *tipnum = tl_int_to_str(n->tip);
            tl_strcat(result, tipnum, true);
        }
        return;
    }
    
    tl_str_append_char(result, '(');
    
    if (n->right) {
        tl_write_Newick_traversal(n->left, dolabels, result, labels);
        tl_str_append_char(result, ',');
        tl_write_Newick_traversal(n->right, dolabels, result, labels);
    } else {
        TLnode* p = n->left;
        while (p) {
            
            tl_write_Newick_traversal(p, dolabels, result, labels);
            
            if (p->sib) {
                tl_str_append_char(result, ',');
            }
            p = p->sib;
        }
    }
    
    tl_str_append_char(result, ')');
}

char* tl_write_Newick(TLtree* t, bool uselabels)
{

    char *nwktree = NULL;
    
    
    return nwktree;
}

