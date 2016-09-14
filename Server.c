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
int sockfd, newsockfd, port, clilen;
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

// while( (newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen)) )
// {
	newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");

    // printf("Connection accepted");
     
    pthread_t sniffer_thread;
     
    if( pthread_create( &sniffer_thread , NULL ,  createConnection , (void*) newsockfd) < 0)
    {
        perror("could not create thread");
        return 1;
    }
     
    pthread_join( sniffer_thread , NULL);

// }
return 0;
}



