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
#include "SystemDef.h"
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
  //sprintf( buffer, "%s", "webserver");
  //send( connectionFd, buffer, strlen(buffer), 0 );

  // Receive data from Server
	char **arr = NULL;
	split(buffer,' ', &arr);
	printf("Command: %s\n", arr[0]);
	printf("param: %s\n", arr[1]);

	if (arr[0][0] == '0')
	{
	printf("COMMAND: FWD");
		Commandbuffer.adress = MOTORFRONTDRIVE;
		Commandbuffer.category = CATEGORIEGENERAL;
		Commandbuffer.type = CONSIGNESANSREPLIQUES;
		Commandbuffer.PositionLeft = 0xffff;
		Commandbuffer.PositionRight = 0xffff;
		Commandbuffer.SpeedLeft = 5000;
		Commandbuffer.SpeedRight = 5000;
		Commandbuffer.AccelerationLeft = 0xffff;
		Commandbuffer.AccelerationRight = 0xfff;
	}
	else if(arr[0][0] == '1')
	{
	printf("COMMAND: BWD");
	Commandbuffer.adress = MOTORREARDRIVE;
	Commandbuffer.category = CATEGORIEGENERAL;
	Commandbuffer.type = CONSIGNESANSREPLIQUES;
	Commandbuffer.PositionLeft = 0xffff;
	Commandbuffer.PositionRight = 0xffff;
	Commandbuffer.SpeedLeft = 5000;
	Commandbuffer.SpeedRight = 5000;
	Commandbuffer.AccelerationLeft = 0xffff;
	Commandbuffer.AccelerationRight = 0xfff;
	}
	else if(arr[0][0] == '2')
	{
	printf("COMMAND: CW");	
	}
	else if(arr[0][0] == '3')
	{
	printf("COMMAND: CCW");	
	}	  
	else if(arr[0][0] == '4')
	{
	printf("COMMAND: CAM");	
	}
	  else
	  {
		printf("nothing");  
	  }
	  	printf("\n");
	sprintf( buffer, "%s", "" );
  	if(recv(connectionFd, buffer, MAX_BUFFER, 0) <= 0 )
	{
				#ifdef WIN32
		closesocket(connectionFd);
		#else
		close(connectionFd);
		#endif

		printf("Client closed.\n");
		exit(1);
	}
  

	/*if (buffer[0] = 'f'){
		
	}*/




}
		#ifdef WIN32
		closesocket(connectionFd);
		#else
		close(connectionFd);
		#endif

		printf("Client closed.\n");
		exit(1);
	return;
}

int split(const char *str, char c, char ***arr)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}


