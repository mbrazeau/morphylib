#include <stdio.h>
#include <stdlib.h>

#include "../src/mplerror.h"
#include "../src/mpl.h"
#include "./ctreelib/treelib.h"
#include "./ctreelib/tltree.h"


#define pfail \
    printf("[  FAIL  ] %s, line: %i in: %s\n", __FUNCTION__, __LINE__, __FILE__)

#define ppass printf("[  PASS  ] %s\n", __FUNCTION__)

#define psumf(...) \
    printf("[  ** FAILED ** ] %i times. Review output to see details.\n", fails)

#define psump printf("[  ** PASSED ** ] All tests passed.\n")

#define theader(testname) printf("\n\n\t%s\n\n", testname);

int test_do_fullpass_on_tree(TLtree* t, Morphy m);

//typedef struct {
//    MPLchtype   userchtype;
//    int         numoftype;
//    int*        indices;
//} typesetter;
//
//typedef struct {
//    int         numtax;
//    int         numchar;
//    char*       symbols;
//    char*       usrmatrix;
//    MPLgap_t    usrgaphandl;
//    typesetter* typesetters;
//    int         numtypes;
//    
//} testdata;
