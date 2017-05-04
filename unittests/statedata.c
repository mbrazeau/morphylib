#include <stdio.h>
#include "../mpl.h"
#include "../statedata.h"

void test_get_states(void)
{
	printf("Testing state getting and conversion:\n");

	char *rawmatrix = "001101--- -100? ?[xyz876jkjk]ABaC434599???";
	char *states = "01ABaC4359";

	Morphy m1 = mpl_new_Morphy();

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
	return 0;
}
