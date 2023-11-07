// THIS CODE SHOULD NOT BE SUBMITTED FOR THE LAB
// Benjamin Belandres

#include "tpool.h"
#include <stdio.h>

int main() {
	
//	printf("%d\n", math(1, 2));
	void *handle = thread_pool_init(3);
	printf("My handle is at %p\n", handle);
	return 0;
}
