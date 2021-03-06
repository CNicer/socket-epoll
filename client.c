#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(3000);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	if(res == -1)
	{
		perror("connect error");
	}
	assert(res != -1);
	
	char buf[128];
	while(1)
	{
		scanf("%s",buf);
		int n = send(sockfd,buf,strlen(buf),0);
		if(n == -1)
			printf("error");
		else
			printf("succes");
	}
	close(sockfd);
}
