#include <stdio.h>
#include "../mpl.h"
#include "../statedata.h"

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
0-00{01}00-10;";
	
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

void test_symboldict(void)
{
	//
}

int main (void)
{
	test_get_states();
	test_count_applic_inapplic();
	return 0;
}
