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

void setAddr(struct sockaddr_in &servaddr,sa_family_t family,in_port_t port)
{
        bzero(&servaddr,sizeof(servaddr));//bzero将套接口地址初始化为0
        servaddr.sin_family = family;
        servaddr.sin_port = port;
}
void msgToSend(FILE *fp,int sockfd)
{
        char sendline[MAXLINE],recvline[MAXLINE];
        fd_set rset;
        FD_ZERO(&rset);
        int stdineof = 0;//if = 0,input continue  if = 1,it is already eof
        while(true)
        {
                FD_SET(sockfd,&rset);
                FD_SET(fileno(fp),&rset);
                int maxfdp1 = max(fileno(fp),sockfd)+1;
                Select(maxfdp1,&rset,NULL,NULL,NULL);
                if(FD_ISSET(sockfd,&rset))
                {
                        if(read(sockfd,recvline,MAXLINE) == 0)
                        {
                                if(stdineof == 1) return;
                                else 
                                {
                                        printf("server terminated prematurely!");
                                        return;
                                }
                        }
                        fputs(recvline,stdout);
                }
                if(FD_ISSET(fileno(fp),&rset))
                {
                        if(fgets(sendline,MAXLINE,fp) == NULL)
                        {
                                stdinof = 1;
                                Shutdown(sockfd,SHUT_WR);
                                FD_CLR(fileno(fp),&rset);
                                continue;
                        }
                        write(sockfd,sendline,strlen(sendline));
                }
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
        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in servaddr;
        setAddr(servaddr,AF_INET,htons(SERV_PORT));
        if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr.s_addr) == 1)
        {
                connect(sockfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
        }
        // msgToSend(stdin,sockfd[0]);
        msgToSend(stdin,sockfd);
        exit(0);
}

