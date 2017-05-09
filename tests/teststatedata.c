#include <stdio.h>
#include "../src/mplerror.h"
#include "../src/mpl.h"
#include "../src/statedata.h"

void test_get_states(void)
{
	printf("Testing state getting and conversion:\n");

	char *rawmatrix = "001101--- -100? ?[xyz876jkjk]ABaC434(59)9???";
	char *states = "01ABaC4359";

	Morphy m1 = mpl_new_Morphy();
	
	int ntax = 5;
	int nchar = 10;
	mpl_init_Morphy(ntax, nchar, m1);
	mpl_attach_rawdata(rawmatrix, m1);
	mpl_delete_Morphy(m1);
}

void test_good_symbols(void)
{
	char *states1 = "0125";
	char *states2 = "0125";
	char *states3 = "1052";
	char *states4 = "2015";

	if (mpl_compare_symbol_lists(states1, states2)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	
	if (mpl_compare_symbol_lists(states2, states1)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	
	if (mpl_compare_symbol_lists(states1, states3)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	
	if (mpl_compare_symbol_lists(states2, states3)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	

	if (mpl_compare_symbol_lists(states1, states4)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
}

void test_bad_symbols(void)
{
	char *states1 = "0125";
	char *states2 = "9125";
	char *states3 = "1082";
	char *states4 = "205";

	if (!mpl_compare_symbol_lists(states1, states2)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	
	if (!mpl_compare_symbol_lists(states2, states1)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	
	if (!mpl_compare_symbol_lists(states1, states3)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	
	if (!mpl_compare_symbol_lists(states2, states3)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}
	

	if (!mpl_compare_symbol_lists(states1, states4)) {
		printf("FAIL:\n");
	}
	else {
		printf("PASS:\n");
	}

	
}

void test_count_applic_inapplic(void)
{
	printf("Testing counting of applicables and inapplicables\n");
	int ntax	= 6;
	int nchar	= 10;
	char *rawmatrix = 
"0000000010\
0-001-22-0\
0-001-110-\
10(03)0101100\
1-000-0000\
0-00{01}100-0;";
	
	int expNA[] = {1,5};
	int nexpNA  = 2;
	int expAP[] = {0,2,3,4,6,7,8,9};
	int nexpAP	= nchar - nexpNA;

	Morphy m1 = mpl_new_Morphy();
//	mpl_set_numtaxa(ntax, m1);
//	mpl_set_num_charac(nchar, m1);
	mpl_init_Morphy(ntax, nchar, m1);	
	mpl_attach_rawdata(rawmatrix, m1);

	mpl_delete_Morphy(m1);
}

void test_load_symbols(void)
{

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
		printf("FAIL\n");
	}
	else {
		printf("PASS\n");
	}
	printf("Loading symbols first\n");
	Morphy m2 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m2);
	
	err = mpl_attach_symbols(symbols, m2);
	if (err) {
		printf("FAIL\n");
	}
	else {
		printf("PASS\n");
	}

	err = mpl_attach_rawdata(rawmatrix, m2);

	if (err) {
		printf("FAIL\n");
	}
	else {
		printf("PASS\n");
	}
	
	if (strcmp(symbols, mpl_get_symbols(m2))) {
		printf("FAIL\n");
	}
	else {
		printf("PASS\n");
	}
}

void test_bad_load_symbols(void)
{

	printf("\n\n\t Load bad symbols \n\n");
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
		printf("FAIL\n");
	}
	else {
		printf("PASS\n");
	}

	printf("\nLoading symbols first\n");
	Morphy m2 = mpl_new_Morphy();
	mpl_init_Morphy(ntax, nchar, m2);
	
	err = mpl_attach_symbols(symbols, m2);
	if (err) {
		printf("FAIL\n");
	}
	else {
		printf("PASS\n");
	}

	err = mpl_attach_rawdata(rawmatrix, m2);

	if (!err) {
		printf("FAIL\n");
	}
	else {
		printf("PASS\n");
	}
}

void test_multistate_symbols(void)
{

	printf("\n\n\tComplex multistate symbols \n\n");
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
}


int main (void)
{
	test_get_states();
	test_count_applic_inapplic();
	test_good_symbols();
	test_bad_symbols();
	test_load_symbols();
	test_bad_load_symbols();
	test_multistate_symbols();
	return 0;
}
