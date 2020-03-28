#include <stdio.h>
#include <sys/syscall.h>

int main(int argc, char *argv)
{
	printf("%d\n", syscall(439, 1, 1)); /* WRONG */
	printf("%d\n", syscall(439, 0xDEACE042, 0xDEAD)); /* GOOD */
	return 0;
}
