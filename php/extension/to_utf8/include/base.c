#include <stdio.h>
int main()
{
	int *p;
	p[0] = 0x0000;
	p[1] = 0x0001;
	p[2] = 0x0002;

	int i;
	for(i = 0; i < 3; i++) {
		printf("%x\n", *(p + i));
	}
}
