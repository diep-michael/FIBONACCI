// Michael Diep, 015330547

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int status;

	if (argc > 2 || (atoi(argv[1]) < 0 )) 	// if there are 2 arguments, or a negative argument
	{
		perror("Incorrect number of arguments used");
		exit(1);
	}
	
	pid_t  pid;
	
	pid = fork();

	const char *numFib = argv[1];		// create variable to pass into child process
	
	if (pid == 0)
	{
		execl("/home/michaeld/Desktop/sh_p.out","sh_p.out", numFib, (char *)NULL);
		printf("prod, pid = %u\n", getpid()); 	// error checking
		perror("Exec Prod Failure");
		exit(1);
	}
	if (pid > 0)		
	{
		pid = wait(&status);		// wait for producer process to finish
		if (pid < 0)
		{
			perror("wait failure"); 
			exit(1);
		}

		pid = fork();
		if (pid == 0)
		{
			execl("/home/michaeld/Desktop/sh_c.out","sh_c.out", numFib, (char *)NULL);
			printf("cons, pid = %u\n", getpid());
			perror("Exec Cons Failure");
			exit(1);
		}
		if (pid > 0)		
		{
			pid = wait(&status);	// wait for consumer process to finish
			if (pid < 0)
			{
				perror("wait failure");
				exit(1);
			}
			exit(0);
		}
		else
		{	
			perror("Fork Failure\n");
			exit(1);
		}
	}
	else
	{	
		perror("Fork Failure\n");
		exit(1);
	}
	return 0;
}

