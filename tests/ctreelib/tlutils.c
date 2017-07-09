#include "tldefs.h"
#include "tlnewick.h"
#include "treelib.h"
#include "tlerror.h"

//#include <stdio.h>

void *tl_alloc(size_t size, int setval)
{
	void *ret = malloc(size);
	if (ret) {
		memset(ret, setval, size);
	}
	return ret;
}

int tl_calc_nnodes(const int ntaxa)
{
    return 2 * ntaxa - 1;
}

int tl_num_nodes_vertices(const int ntaxa)
{
    return ntaxa - 1;
}

int tl_check_duplicate_label(const char* newlab, const TLstrlist* labels)
{
    int i = 0;
    int max = labels->nelems;
    int ret = 1;
    
    for (i = 0; i < max; ++i) {
        if(!strcmp(newlab, labels->labels[i]->label)) {
            ret = 0;
            break;
        }
    }
    
    return ret;
}


int tl_get_label_index(const TLlabel *label)
{
    return label->nodeindex;
}

char* tl_int_to_str(long i)
{
    int len = (int) (ceil(log10(i)) + 2);
    
    char* res = (char*)malloc(len * sizeof(char));
    
    snprintf(res, len * sizeof(char), "%ld", i);
    if (len-2 != 0) {
        res[len-2] = '\0';
    }
    else {
        res[len-1] = '\0';
    }
    return res;
}

char* tl_strcat(char** str1, char* str2, bool delete2)
{
    unsigned long len = strlen(*str1) + strlen(str2);
    ++len;
    
    char *res = (char*)realloc(*str1, len * sizeof(char));
    if (!res) {
        return NULL;
    }
    
    strcat(res, str2);
    *str1 = res;
    if (delete2) {
        free(str2);
    }
    
    return *str1;
}

char* tl_str_append_char(char **str, const char c)
{
    unsigned long len = strlen(*str);
    ++len;
    
    char* result = NULL;
    result = (char*)realloc(*str, (len+1) * sizeof(char));
    if (!result) {
        return NULL;
    }
    
    result[len-1] = c;
    result[len] = '\0';
    
    *str = result;
    
    return *str; // Return the address of the last point
}

//int tl_compr_by_index(const void* ptr1, const void *ptr2)
//{
//    TLlabel *lab1 = (TLlabel*)ptr1;
//    TLlabel *lab2 = (TLlabel*)ptr2;
//    return lab1->nodeindex - lab2->nodeindex;
//}


int tl_push_label(TLlabel *newlab, TLstrlist *strlst)
{

    
    if (!strlst->labels) {
        strlst->labels = (TLlabel**)tl_alloc(sizeof(TLlabel*), 0);
        if (strlst->labels) {
            strlst->maxelems = 1;
        }
        else {
            return -1; // TODO: ERR CODE
        }
    }
    else {
        TLlabel** temp = NULL;
        int newsize = strlst->nelems + 1;
        temp = (TLlabel**)realloc(strlst->labels, newsize * sizeof(TLlabel*));
        if (temp) {
            strlst->labels = temp;
            ++strlst->maxelems;
        }
        else {
            return -2; // TODO: ERR CODE REALLOC FAIL
        }
    }
    
    int asindex = 0;
    if ((asindex = atoi(newlab->label))) {
        newlab->nodeindex = asindex;
    }
    else {
        newlab->nodeindex = strlst->nelems;
    }
    strlst->labels[strlst->nelems] = newlab;
    ++strlst->nelems;
    
    return 0;
}


int tl_push_new_label(const char* str, TLstrlist *strlst)
{
    int ret = 0;
    if (strlst) {
        if (str) {
            
            TLlabel *newlab = (TLlabel*)tl_alloc(sizeof(TLlabel), 0);
            
            if (newlab) {
                
                newlab->labellen = strlen(str);
                newlab->label = (char*)tl_alloc((newlab->labellen + 1) *
                                                sizeof(char), 0);

                if (newlab->label) {
                    
                    // TODO: Give this an error check.
                    strcpy(newlab->label, str);
                    tl_push_label(newlab, strlst);
                    
                } else {
                    ret = -1;
                }
            } else {
                ret = -1;
            }
        } else {
            ret = -2; // TODO: ERR CODE
        }
    } else {
        ret = -1; // TODO: ERR CODE
    }
    return ret;
}


char* tl_getc(const char *str)
{
	static char	*c;
	static char	*mmbr;
	
	if (str != mmbr) {
		c = (char*)str;
		mmbr = (char*)str;
	}

	if (*c == '\0') {
//		c = NULL;
		mmbr = NULL;
		return c;
	}

	return c++;
}


char* tl_strdup(const char* str)
{
	char *ret = (char*)malloc(strlen(str) + 1);
	if (!ret) return NULL;
	strcpy(ret, str);
	return ret;
}

int tl_lookup_label(int asint, char* aslabel, TLhandlp handl)
{
    TLstrlist* taxlabels = &handl->taxlabels;
  
    int i = 0;
    int nelems = taxlabels->nelems;
    
    if (aslabel) {
       for (i = 0; i < nelems; ++i) {
            if (!strcmp(aslabel, taxlabels->labels[i]->label)) {
                return taxlabels->labels[i]->nodeindex;
            }
        }
    } else {
        for (i = 0; i < nelems; ++i) {
            if (asint == taxlabels->labels[i]->nodeindex) {
                return taxlabels->labels[i]->nodeindex;
            }
        }
    }
    return ERR_TGT_NOT_FOUND;
}
