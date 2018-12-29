/*多个客户端client*/
#include "unp.h"
#include "error.c"
/*struct in_addr
{
        in_addr_t s_addr;//ip
}
struct sockaddr_in
{
        sa_family_t sin_family;
        in_port_t sin_port;//port
        struct in_addr sin_addr;//ip
        unit8_t sin_len;
        char sin_zero[8];
};*/

void setAddr(struct sockaddr_in &servaddr,sa_family_t family,in_addr_t addr,in_port_t port)
{
        bzero(&servaddr,sizeof(servaddr));//bzero将套接口地址初始化为0
        servaddr.sin_family = family;
        servaddr.sin_addr.s_addr = addr;//通配地址由INADDR_ANY指定，通知内核选择ip地址
        servaddr.sin_port = port;
}
void msgToSend(FILE *fp,int sockFd)
{
        char sendline[MAXLINE],recvline[MAXLINE];
        while(fgets(sendline,MAXLINE,fp) != NULL)
        {
                char timeToSend[MAXLINE+20];
                time_t ticks = time(NULL);
                snprintf(timeToSend,sizeof(timeToSend),"%.24s",ctime(&ticks));
                strcat(timeToSend," ");
                strcat(timeToSend,sendline);
                strcpy(sendline,timeToSend);
                write(sockFd,sendline,strlen(sendline));
                if(read(sockFd,recvline,MAXLINE) == 0)
                {
                        printf("read msg error!");
                        return;
                }
                fputs(recvline,stdout);
                memset(sendline,0,sizeof(sendline));
                memset(recvline,0,sizeof(recvline));
        }
}
int main(int argc,char** argv)
{
        if(argc != 2)
        {
                printf("输入错误，需要输入ip地址\n");
                exit(0);
        }
        int sockfd[5];	
	for(int i = 0;i < 5;i++)
	{
		sockfd[i] = socket(AF_INET,SOCK_STREAM,0);
		struct sockaddr_in servaddr;
                setAddr(servaddr,AF_INET,htonl(INADDR_ANY),htons(SERV_PORT));
                if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr.s_addr) != 1)
                {
                        printf("error address");
                        exit(0);
                }
                connect(sockfd[i],(struct sockaddr*) &servaddr,sizeof(servaddr));
	}
        msgToSend(stdin,sockfd[0]);
        exit(0);
}

