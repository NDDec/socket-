/*服务器server*/
#include "unp.h"
#include "apue.h"

void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
		printf("child %d terminated\n",pid);
	return;
}

void setAddr(struct sockaddr_in &servaddr,sa_family_t family,in_addr_t addr,in_port_t port)
{
	bzero(&servaddr,sizeof(servaddr));//bzero将套接口地址初始化为0
	servaddr.sin_family = family;
	servaddr.sin_addr.s_addr = addr;//通配地址由INADDR_ANY指定，通知内核选择ip地址
	servaddr.sin_port = port;
}

void strToEcho(int sockFd)
{
	char msg[MAXLINE];
	while(true)
	{
		ssize_t readLen = 0;
		readLen = read(sockFd,msg,MAXLINE);
		if(readLen = 0) return;
		write(sockFd,msg,strlen(msg));
		memset(msg,0,sizeof(msg));
	}
}

int main(int argc,char** argv)
{
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in servaddr;
	setAddr(servaddr,AF_INET,htonl(INADDR_ANY),htons(SERV_PORT));
	bind(listenfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
	listen(listenfd,LISTENQ);//允许LISTENQ来覆盖调用者所给的值,1024

	signal(SIGCHLD,sig_chld);//waitpid()处理僵尸进程
	while(true)
	{
		struct sockaddr_in cliaddr;
		socklen_t len = sizeof(cliaddr);
		int connfd;
		if((connfd = accept(listenfd,(struct sockaddr*) &cliaddr,&len)) < 0)
		//在这里会阻塞，直到建立起连接
		{
		   if(errno == EINTR) continue;
		   else printf("accept error");
		}
		pid_t childpld;
		if((childpld = fork()) == 0)
		{
			close(listenfd);
			strToEcho(connfd);
			exit(0);
		}
		close(connfd);
	}
}
