// call_socket.c - A sample program to demonstrate the TCP client
//
#ifdef WIN32
#include <sys/types.h>
#include <Winsock2.h>
#define	WINSOCKVERSION	MAKEWORD( 2,2 )		
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#endif

#include <stdio.h>
#include <string.h>

#include "CommunicationWebServer.h"

#define MAX_BUFFER		128
#define HOST			"127.0.0.1"
#define PORT     		50007

void CommunicationWebServer ()
{
  int connectionFd, rc, index = 0, limit = MAX_BUFFER;
  struct sockaddr_in servAddr, localAddr;
  char buffer[MAX_BUFFER+1];

#ifdef WIN32
  // Start up WinSock2
  WSADATA wsaData;
  if( WSAStartup( WINSOCKVERSION, &wsaData) != 0 ) return ERROR;		
#endif

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT);
  servAddr.sin_addr.s_addr = inet_addr(HOST);

  // Create socket
  connectionFd = socket(AF_INET, SOCK_STREAM, 0);

  /* bind any port number */
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(0);
  
  rc = bind(connectionFd, (struct sockaddr *) &localAddr, sizeof(localAddr));

  // Connect to Server
  connect(connectionFd,
           (struct sockaddr *)&servAddr, sizeof(servAddr));
  int i=0;

  while(1){

// In this loop, we interpret the events/commands gathered by the socket communicating with the Python Module.
//The purpose is just to fill out the apropriate fields in the structure "Commandbuffer" defined in SystemDef.h
// Make sure to use the appropriate Wait and post semaphore mecanism to fill the producer/consummer queue with the message
// strutured by the Commandbuffer structure.

// The following sequence of sending following by the receiving is just for testing purpose

  // Send request to Server
  //sprintf( buffer, "%s", "Hello, Server!" );
  sprintf( buffer, "%i", i);
  send( connectionFd, buffer, strlen(buffer), 0 );
  i++;
  //printf("Client sent to sever \"%s\"\n", buffer);

  // Receive data from Server
  sprintf( buffer, "%s", "" );
  recv(connectionFd, buffer, MAX_BUFFER, 0);
  //printf("Client read from Server \"%s\"\n", buffer);

	/*if (buffer[0] = 'f'){
		
	}*/




}
#ifdef WIN32
  closesocket(connectionFd);
#else
  close(connectionFd);
#endif
  
  printf("Client closed.\n");

  return;
}


