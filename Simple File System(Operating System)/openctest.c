#include "call.h"

int main (int argc, char *argv[])
{
	char filename[5][MAX_COMMAND_LENGTH] = {"/", "/dir5", "/dir5/dir1", "/dir5/dir1/file1", "/dir5/dir8/dir12/file2"};
	char test[5]="/dir5";
	int expected[5] = {0, 1, 5, 13, 14};

	//Start testing

		int i_number = open_t("/dir5");
		printf("======case open \'%s\' =======\n", test);
		printf("returned inode number: %d\t\n\n", i_number);
	return 0;
}
