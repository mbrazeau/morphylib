#include <stdio.h>
#include "../src/morphydefs.h"
#include "../src/mpl.h"

int main (void)
{
	printf("Testing new Morphy obj...\n");
	Morphy new = mpl_new_Morphy();
	return 0;
}
