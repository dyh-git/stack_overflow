#include <unistd.h>
#include <stdio.h>
#include <string.h>

void test_stack_overflow(void)
{
	char a[2] = {0xab, 0xcd};
	
	printf("%s, stack addr %p\n", __func__, a);
	
	strcpy(a, "are you ok.\n");
	
	return;
}
