#include <stdio.h>
#include "mpltest.h"
#include "../src/statedata.h"

int test_get_states(void)
{

    theader("State-extraction and conversion");
    
    int failn = 0;
    
	char *rawmatrix = "001101--- -100? ?[xyz876jkjk]ABaC434(59)9???";
//	char *states = "01ABaC4359";
    char *retstats;
    
	Morphy m1 = mpl_new_Morphy();
	
	int ntax = 5;
	int nchar = 10;
    
	mpl_init_Morphy(ntax, nchar, m1);
	mpl_attach_rawdata(rawmatrix, m1);
    retstats = mpl_get_symbols(m1);
    printf("Retstates: %s\n", retstats);
    
    if (retstats) {
        pfail;
        ++failn;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m1);
    
    return failn;
}

int test_good_symbols(void)
{
    theader("Load good (non-conflicting) symbols");
    int failn = 0;
    
	char *states1 = "0125";
	char *states2 = "0125";
	char *states3 = "1052";
	char *states4 = "2015";

	if (mpl_compare_symbol_lists(states1, states2)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	
	if (mpl_compare_symbol_lists(states2, states1)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	
	if (mpl_compare_symbol_lists(states1, states3)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	
	if (mpl_compare_symbol_lists(states2, states3)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	

	if (mpl_compare_symbol_lists(states1, states4)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
    
    return failn;
}

int test_bad_symbols(void)
{
    theader("Conflicting symbols lists");
    int failn = 0;
    
	char *states1 = "0125";
	char *states2 = "9125";
	char *states3 = "1082";
	char *states4 = "205";

	if (!mpl_compare_symbol_lists(states1, states2)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	
	if (!mpl_compare_symbol_lists(states2, states1)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	
	if (!mpl_compare_symbol_lists(states1, states3)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	
	if (!mpl_compare_symbol_lists(states2, states3)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	

	if (!mpl_compare_symbol_lists(states1, states4)) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}

	return failn;
}

int test_count_applic_inapplic(void)
{
	printf("Testing counting of applicables and inapplicables\n");
    int failn = 0;
	int ntax	= 6;
	int nchar	= 10;
	char *rawmatrix = 
"0000000010\
0-001-22-0\
0-001-110-\
10(03)0101100\
1-000-0000\
0-00{01}100-0;";
	
//	int expNA[] = {1,5};
//	int nexpNA  = 2;
//	int expAP[] = {0,2,3,4,6,7,8,9};
//	int nexpAP	= nchar - nexpNA;

	Morphy m1 = mpl_new_Morphy();
//	mpl_set_numtaxa(ntax, m1);
//	mpl_set_num_charac(nchar, m1);
	mpl_init_Morphy(ntax, nchar, m1);	
	mpl_attach_rawdata(rawmatrix, m1);

	mpl_delete_Morphy(m1);
    
    return failn;
}


int test_load_symbols(void)
{

    theader("Load symbols");
    
    int failn = 0;
	int ntax	= 6;
	int nchar	= 10;
	char *rawmatrix = 
"0000000010\
0-001-22-0\
0-001-110-\
10(03)0101100\
1-000-0000\
0-00{01}100-0;";

	char* symbols = "0123";

	MPL_ERR_T err = ERR_NO_ERROR;

	printf("Loading data first\n");
	Morphy m1 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m1);	
	mpl_attach_rawdata(rawmatrix, m1);
	err = mpl_attach_symbols(symbols, m1);
	mpl_delete_Morphy(m1);


	if (err) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	printf("Loading symbols first\n");
	Morphy m2 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m2);
	
	err = mpl_attach_symbols(symbols, m2);
	if (err) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}

	err = mpl_attach_rawdata(rawmatrix, m2);

	if (err) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
	
	if (strcmp(symbols, mpl_get_symbols(m2))) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
    
    return failn;
}

int test_bad_load_symbols(void)
{

    theader("Load bad symbols");

    int failn   = 0;
	int ntax	= 6;
	int nchar	= 10;
	char *rawmatrix = 
"0000000010\
0-001-22-0\
0-001-110-\
10(03)0101100\
1-000-0000\
0-00{01}100-0;";

	char* symbols = "012";

	MPL_ERR_T err = ERR_NO_ERROR;

	printf("Loading data first\n");
	Morphy m1 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m1);	
	mpl_attach_rawdata(rawmatrix, m1);
	err = mpl_attach_symbols(symbols, m1);
	mpl_delete_Morphy(m1);

	if (!err) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}

	printf("\nLoading symbols first\n");
	Morphy m2 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m2);
	
	err = mpl_attach_symbols(symbols, m2);
	if (err) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}

	err = mpl_attach_rawdata(rawmatrix, m2);

	if (!err) {
        ++failn;
        pfail;
	}
	else {
        ppass;
	}
    return failn;
}

int test_usr_order_symbols(void)
{
    
    theader("Test user order of symbols");
    
    int failn   = 0;
    int ntax	= 6;
    int nchar	= 10;
    char *rawmatrix =
    "0000000010\
    0-001-22-0\
    0-001-110-\
    10(03)0101100\
    1-000-0000\
    0-00{01}100-0;";
    
    char* symbols = "3201";
    
    MPL_ERR_T err = ERR_NO_ERROR;
    
    printf("Loading data first\n");
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    
    char* resymbols = mpl_get_symbols(m1);
    if (!strcmp(resymbols, symbols)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    err = mpl_attach_symbols(symbols, m1);
    resymbols = mpl_get_symbols(m1);
    
    if (strcmp(resymbols, symbols)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m1);
    
    
    return failn;
}


int test_multistate_symbols(void)
{

	printf("\n\n\tComplex multistate symbols \n\n");
    int failn   = 0;
	int ntax	= 6;
	int nchar	= 10;
	char *rawmatrix = 
"0000000010\
0-00(01)(02)-22-\
0-0{01}(13)1-110\
10(03){02}110110\
1-000-0000\
0-00{01}{13}10-0;";

	char* symbols = "012";

	MPL_ERR_T err = ERR_NO_ERROR;

	printf("Bad states:\n");
	Morphy m1 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m1);	
	
	mpl_attach_rawdata(rawmatrix, m1);
	err = mpl_attach_symbols(symbols, m1);

	mpl_delete_Morphy(m1);

	symbols = "0123";
	printf("\nGood states:\n");
	Morphy m2 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m2);	
	
	mpl_attach_rawdata(rawmatrix, m2);
	err = mpl_attach_symbols(symbols, m2);
	
	mpl_delete_Morphy(m2);
    
    return failn;
}

