#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h> 

void error(char *m)
{
perror(m);
}

void *createConnection(void *newsockfd){
	int n;
	void* buffer[256];
	n = read(newsockfd,buffer,255);
	int d=atoi(buffer)*5;

	if (n < 0) error("ERROR reading from socket");
	printf("Message received:%d\n",d);
	sprintf(buffer, "%d", d);
	n = write(newsockfd,buffer, strlen((char *)buffer));
	if (n < 0)
	error("ERROR writing back to socket");
}

int main(int argc, char *argv[])
{ 
int sockfd, newsockfd, port, clilen, pid, pidb, pidc;
struct sockaddr_in serv_addr, cli_addr;

if (argc < 2)
error("ERROR, no port provided\n");
port = atoi(argv[1]);

sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
error("ERROR opening socket");

bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;
serv_addr.sin_port = htons(port); //host to network

if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
error("ERROR binding to socket");

listen(sockfd,2);
clilen = sizeof(cli_addr);
newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen);
pthread_t t1,t2,t3;
    if( pthread_create( &t1 , NULL ,  createConnection , (void*) newsockfd) < 0)
    {
        perror("could not create thread 1");
        return 1;
    }
    if( pthread_create( &t2 , NULL ,  createConnection , (void*) newsockfd) < 0)
    {
        perror("could not create thread 2");
        return 1;
    }
    if( pthread_create( &t3 , NULL ,  createConnection , (void*) newsockfd) < 0)
    {
        perror("could not create thread 3");
        return 1;
    }
    pthread_join( t1 , NULL);
    pthread_join( t2 , NULL);
    pthread_join( t3 , NULL);



return 0;
}



