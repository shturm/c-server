#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

int main (int argc, char *argv[])
{
  int s, t, sinlen;
  struct sockaddr_in sin;
  int i;
  char msg[80];
  struct hostent * hp;
  int j;

  if (argc < 2) { 
    printf("%s port\n", argv[0] ); /* input error: need port no! */
    return -1;
  }

  /* First call to socket() function */
  // sockfd = socket(int socket_family, int socket_type, int protocol); 
  if ( (s = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) { /* create socket*/
    printf("error on socket\n");  /* socket error */
    return -1;
  }
  
  /* Initialize socket structure */
  bzero((char *) &sin, sizeof(sin));    /* set zeros to structure */
  sin.sin_family = PF_INET;             /* set protocol family to Internet */
  sin.sin_port = htons(atoi(argv[1]));  /* set port no. */
  sin.sin_addr.s_addr  = INADDR_ANY;    /* set IP addr to any interface */
  
  /* Now bind the host address using bind() call.*/
  if (bind(s, (struct sockaddr *)&sin, sizeof(sin) ) < 0 ) {
    printf("error on bind\n"); return -1;  /* bind error */
  }
  
   /* Now start listening for the clients, here
      * process will go in sleep mode and will wait
      * for the incoming connection
   */
  if (listen(s, 5)) { 
    printf ("error on listen\n"); /* listen error*/
    return -1;
  }
  sinlen = sizeof(sin);
  
  while (1) {
    /* accepting new connection request from client,
    socket id for the new connection is returned in t */
    if ( (t = accept(s, (struct sockaddr *) &sin, &sinlen) ) < 0 ) {
      printf("error on accept\n");  /* accept error */
      return -1;
    }
    
	/* communication is accepted, we want to get information about the remote device */
	/* The gethostbyname() function returns a structure of type hostent for the given host name. */
	hp=(struct hostent *)gethostbyaddr((char *)&sin.sin_addr,4,AF_INET);
    //j=(int)(hp->h_length);
    printf( "From %s (%s) :%d.\n",inet_ntoa(sin.sin_addr), hp->h_name, ntohs(sin.sin_port) );
    
	
	bzero(msg,sizeof(msg));              // clear array
    if ( read(t, msg, sizeof(msg) ) <0) {  /* read message from client */
      printf("error on read\n");         /*  read error */
      return -1;
    }
    printf("length of message is %d\nmessage from client is: %s\n",strlen(msg),msg);
    if ( write(t, msg, strlen(msg) ) < 0 ) {  /* echo message back */
      printf("error on write\n");    return -1; /*  write error */
    }
    /* close connection, clean up sockets */
    if (close(t) < 0) { printf("error on close"); return -1;} 
  } // not reach below
  if (close(s) < 0) { printf("close"); return -1;}
  return 0;
}
