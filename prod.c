// Michael Diep, 015330547

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>

int convDigits(int num)
{
	int c = 1;
	while (num > 9){
		c++;
		num /= 10;
	}
	return c; // counts # of digits in the number
}

int main(int argc, char *argv[]){
	
	if (argc == 1)
	{
		printf("\nUsage: %s <number_to_generate>\n", argv[0]); // if no argument is found
		exit(-1);
	} 
	const int SIZE = 4096;
	const char *name = "Fibonacci";
	int shm_fd;
	int n, i;
	n = atoi(argv[1]);
	unsigned int fib[SIZE];
	void *ptr;
	fib[0] = 1;		// initial numbers of sequence
	fib[1] = 1;		//
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);	// create shared memory, read and write
	if(shm_fd == -1){
		printf("Shared memory failed\n");
		exit(-1);
	}
	ftruncate(shm_fd,SIZE);					// truncate memory
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); // maps address space
	if(ptr == MAP_FAILED){
		printf("Map failed\n");
		exit(-1);
	}
	sprintf(ptr,"%d", fib[0]);				// Adds initial
	ptr += convDigits(fib[0]);				// numbers
	sprintf(ptr,"%s", ", ");				// to
	ptr += 2;						// the
	sprintf(ptr,"%d", fib[1]);				// sequences
	ptr += convDigits(fib[1]);				//
	
	if (n >= 1)
	{
		for (i = 2; i<(n+2); i++)
		{	sprintf(ptr,"%s", ", ");
			ptr += 2;
			fib[i] = fib[i-1] + fib[i-2];		// adds each additional number to sequence
			sprintf(ptr,"%d", fib[i]);		// depending on argv[1]
			ptr += convDigits(fib[i]);	
		}
	}
	sprintf(ptr,"\n");
	
	return 0;
}


