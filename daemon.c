#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(pid > 0)
		eixt(0);
	if(pid == 0)
	{
		int sid = setsid();
		execl("./server",'\0');	
		chdir("/");
		close(0);
		umask(0);
	}
	else if(pid == -1)
		perror("fork error");
	return 0;
}
