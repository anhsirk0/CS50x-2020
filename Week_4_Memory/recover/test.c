#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(void)
{
	BYTE x = 0xe0;
	printf("%d\n",0xe0);
	printf("%d\n",0xef);
	printf("%d\n",0xd8);
	printf("%d\n",0xff);

	if (x == 224)
	{
		printf("yes \n");
	}
	
}
