#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/epoll.h>

#define maxfd 1000

int socketinit()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(3000);
 	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr)) == -1)
		return -1;
	if(listen(sockfd,10)== -1)
		return -1;
	return sockfd;
}

int addfd(int epfd,int fd)
{
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = fd;
	if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event) == -1)
		return -1;
	return 1;
}

int delfd(int epfd,int fd)
{
	close(fd);
	printf("%d disconnected",fd);
	fflush(stdout);
	if(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL) == -1)	
		return -1;
	return 1;	
}


int main()
{
	int sockfd = socketinit();
	if(sockfd == -1)
		perror("sockfd error");
	assert(sockfd != -1);
	int epfd = epoll_create(1);
	addfd(epfd,sockfd);
	struct epoll_event events[maxfd];
	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	while(1)
	{
		int n = epoll_wait(epfd,events,maxfd,2000);
		assert(n!= -1);
		for(int i = 0;i <n ;i++)
		{
			if(events[i].data.fd == sockfd)
			{
				int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
				printf("%d connetcd",c);
				fflush(stdout);
				addfd(epfd,c);
			}
			else
			{
				int c = events[i].data.fd;
				char buf[128] = {0};
				int res = recv(c,buf,128,0);
				if(res == -1||res == 0)
					delfd(epfd,c);
				else
				{
					printf("%s\n",buf);
					fflush(stdout);
				}
			}
		}
	}	
	close(sockfd);
	
}
