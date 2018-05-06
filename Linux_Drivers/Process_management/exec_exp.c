#include<stdio.h>
#include<unistd.h>
#include<wait.h>

int main (int argc,char *argv[])
{
	pid_t frk;
	printf("Inside exec_gen file \n");
	frk=fork();
        if(frk<0)
		perror("Could not generate child \n");

	if(frk==0)
	{
		printf("I am the child with PID = %d and my parent ID = %d\n",getpid(),getppid());

	}
	if(frk != 0)
	{
		printf("I am the child with ID = %d and my parent ID = %d\n",frk,getpid());
	        execl("/bin/ls","ls","-l",(char *)0);

	}
        wait(NULL);
	printf("Ls command completed\n");
	return 0;
}
