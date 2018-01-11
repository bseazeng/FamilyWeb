/*****************************************************************

mymultiwebserver.c 

system:redhat linux Fedora Core 5

enviroment:g++

compile command:g++ -g -o mymultiwebserver -lpthread

date:10/15/2006

By Manio

*****************************************************************/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8848
#define BACKLOG 5
#define BUFF_SIZE 1500
#define DEBUG 1

struct ARG_
{
    int     connfd;
    struct  sockaddr_in client;
};

typedef struct ARG_  ARG;

//get the extent name of the file
char* getextname(char* filepath)
{

    char* p;

    if(( p  =  strrchr(filepath,'.')) != NULL)
        return p+1;

    return NULL;           

}

//send the 404 error message to the client
void msg404(int connectfd)
{
    char* msg;
    
    msg  = "HTTP/1.0 404 Not Found Content-Type: text/plain 404 not found by Manio";
    send(connectfd,msg,strlen(msg),0);

}

//is the filepath a file  or directory
int fileordirExist(char* fpath)
{
    struct stat filestat;
    return (  stat(fpath,&filestat) != -1);

}

//handle the request of the client
void process_cli(int connectfd, struct sockaddr_in client)
{
    int num;
    //char recvbuf[BUFF_SIZE], sendbuf[BUFF_SIZE], cli_name[BUFF_SIZE];
    char requestline[BUFF_SIZE], filepath[BUFF_SIZE], cmd[BUFF_SIZE],extname[BUFF_SIZE];
    int c;
    FILE *fp;
    FILE *cfp;
    
    fp = fdopen(connectfd,"r");  
    
    #ifdef DEBUG
    printf("the host is:%s  \n",inet_ntoa(client.sin_addr) );
    #endif
    
    fgets(requestline,BUFF_SIZE,fp);

    #ifdef DEBUG
    printf(" THE REQUEST IS :%s \n",requestline);
    #endif

    strcpy(filepath,"./");
    sscanf(requestline,"%s%s",cmd,filepath+2);
    strcpy(extname, getextname(filepath));

    #ifdef DEBUG
    printf("cmd:%s filepath:%s extname:%s \n",cmd,filepath,extname);
    printf("string comparing :::::::::::::start::::::::::::::: \n"); 
    #endif

    if(strcmp(cmd,"GET") == 0)
    {

        //the command is get
        #ifdef DEBUG
        printf("cmd(%s)==GET \n",cmd);
        #endif

        //is this a file or dir or notexist?
        if(fileordirExist(filepath))
        {

            //is a file or dir or none
            //is this a dir 
            if(IsDIR(filepath))
            {
                //is a dir
                #ifdef DEBUG
                printf("%s is a DIR \n",filepath);
                #endif

                if( fileordirExist( strcat(filepath,"index.htm") ))
                {
                    sendobj(connectfd,"index.htm");
                }
                else if(fileordirExist(strcat(filepath,"index.html")))
                {
                    sendobj(connectfd,"index.htm");
                }
                else
                {
                    msg404(connectfd);
                }

            }
            else
            {
                //is a file
                #ifdef DEBUG
                printf("%s is a file\n",filepath);
                #endif
                sendobj(connectfd,filepath);
            }

        }
        else
        {
            #ifdef DEBUG
            printf("404 \n");
            #endif
            msg404(connectfd);
        }
    }
    else
    {
        #ifdef DEBUG
        printf("cmd(%s)!=GET \n",cmd);
        #endif
    }

    #ifdef DEBUG
    printf(":::::::::::::end::::::::::::::: \n"); 
    #endif
    
    close(connectfd);

}

//invoked by pthread_create
void* start_routine(void* arg)
{
    ARG *info;

    info = (ARG *)arg;
    //handle client's requirement
    process_cli(info->connfd, info->client);
    free(arg);
    pthread_exit(NULL);

}


// is the filepath a directory
int IsDIR(char* fpath)
{

    #ifdef DEBUG
    printf("IN IsDIR ");
    #endif

    struct stat filestat;
    return ( stat(fpath,&filestat) != -1 && S_ISDIR(filestat.st_mode));

}

//send the data of the file which the client want
int sendobj(int connectfd,char* serverfilepath)
{
    #ifdef DEBUG
    printf("INWRITEHEAD:::::::extname is %s::::::: \n",serverfilepath);
    #endif
    
    FILE* sfp   = NULL;
    char c;
    
    char *http_res_tmpl = "HTTP/1.1 200 OK\r\n"
        "Server: Cleey's Server V1.0\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "Content-Type: %s\r\n\r\n";
     
    char HTTP_HEADER[BUFF_SIZE]    =   {0};
    char HTTP_INFO[BUFF_SIZE]      =   {0};
    int len =   0;
    char content [BUFF_SIZE]     =   {0};
    int content_i = 0;


    
    sfp = fopen(serverfilepath,"r");
    while( (c = getc(sfp)) != EOF)
    {
       content[content_i++]=c;  
    }  
    len = strlen(content);
    sprintf(HTTP_HEADER,http_res_tmpl,len,"text/html");
    printf("content:%s\n",content);
    len = sprintf(HTTP_INFO,"%s%s",HTTP_HEADER,content);

    write(connectfd,HTTP_INFO,len);
    return 0;

}


int main(void)
{

    int listenfd;
    int connectfd;
    pthread_t thread;         //id of thread
    ARG *arg;            //pass this var to the thread
    struct sockaddr_in server; //server's address info
    struct sockaddr_in client; //client's
    int sin_size;
    int opt = SO_REUSEADDR;



    //create tcp socket
    #ifdef DEBUG
    printf("socket.... \n");
    #endif

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("creating socket failed.\n");
        exit(1);
    }
    
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
    printf("bind.... \n");
    if(bind(listenfd,(struct sockaddr *)&server,sizeof(struct sockaddr)) == -1) 
    {
        perror("bind error.\n");
        exit(1);
    }

    printf("listen.... \n");
    if(listen(listenfd,BACKLOG) == -1) 
    {
        perror("listen() error ");
        exit(1);
    }
    
    sin_size = sizeof(struct sockaddr_in);
    while(1)
    {

        //accept() using main thread
        printf("accepting.... \n");
        if((connectfd = accept(listenfd,(struct sockaddr *)&client, (socklen_t*)&sin_size)) == -1) 
        {
            printf("accept() error \n");
        }

        arg = (ARG*)malloc(sizeof(ARG));
        arg->connfd = connectfd;
        memcpy((void *)&arg->client, &client, sizeof(client));

        //invoke start_routine to handle this thread
        #ifdef DEBUG
        printf("thread_creating....\n");
        #endif

        if(pthread_create(&thread, NULL, start_routine, (void*)arg))
        {
            perror("pthread_create() error\n");
            exit(1);
        }        
    }

    close(listenfd); 

}

