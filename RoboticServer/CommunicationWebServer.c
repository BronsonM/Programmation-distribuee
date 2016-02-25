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
	sprintf( buffer, "" );
	if (sem_trywait(&buflockempty)==0)
	{
		
		if (arr[0][0] == '0')
		{

		printf("COMMAND: avancer de %s", arr[1]);
			Commandbuffer.adress = MOTORFRONTDRIVE;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = verifierDef(arr[1],1);
			Commandbuffer.SpeedRight = verifierDef(arr[1],-1);
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		else if(arr[0][0] == '1')
		{
		printf("COMMAND: reculer de %s", arr[1]);
		Commandbuffer.adress = MOTORREARDRIVE;
		Commandbuffer.category = CATEGORIEGENERAL;
		Commandbuffer.type = CONSIGNESANSREPLIQUES;
		Commandbuffer.PositionLeft = 0xffff;
		Commandbuffer.PositionRight = 0xffff;
		Commandbuffer.SpeedLeft = verifierDef(arr[1],1);
		Commandbuffer.SpeedRight = verifierDef(arr[1],-1);
		Commandbuffer.AccelerationLeft = 0xffff;
		Commandbuffer.AccelerationRight = 0xffff;
		}
		else if(arr[0][0] == '2')
		{
			printf("COMMAND: tourner de %s du sens horraire", arr[1]);	
			Commandbuffer.adress = MOTORREARDRIVE;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = verifierDef(arr[1],1);
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == '3')
		{
			printf("COMMAND: tourner de %s du sens anti-horraire", arr[1]);
			Commandbuffer.adress = MOTORREARDRIVE;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = verifierDef(arr[1],1);
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == '4')
		{
			printf("COMMAND: demarrage de la camera");	
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.camON = 1;
		}
		
		else if(arr[0][0] == '5')
		{
			printf("COMMAND: arret de la camera");
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.camON = 0;
		}
		
		//Panning a camera is left, right movement on the horizontal plane. Tilting is up and down movement
		else if(arr[0][0] == '6')
		{
			printf("COMMAND: touner la camera vers le haut");
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.tiltAngle = verifierDefFloat(arr[2],1);
		}
		
		else if(arr[0][0] == '7')
		{
			printf("COMMAND: tourner la camera vers le bas");
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.tiltAngle = verifierDefFloat(arr[2],-1);
		}
		
		else if(arr[0][0] == 'l')
		{
			printf("COMMAND: tourner la camera vers le gauche");
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.panAngle = verifierDefFloat(arr[2],1);
		}
		
		else if(arr[0][0] == 'm')
		{
			printf("COMMAND: tourner la camera vers le droite");
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.panAngle = verifierDefFloat(arr[2],-1);
		}
		
		else if(arr[0][0] == 'n')
		{
			printf("COMMAND: zoomin la camera de %s", arr[1]);	
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.zoomLevel = atof(arr[1]);
		}
		
		else if(arr[0][0] == 'o')
		{
			printf("COMMAND: zoomout la camera de %s", arr[1]);	
			Commandbuffer.adress = CAMERA;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			Commandbuffer.zoomLevel = -atof(arr[1]);
		}	
		
		else if(arr[0][0] == '8')
		{
			printf("COMMAND: tourner le bras avant droit de %s degrees", arr[1]);
			Commandbuffer.adress = MOTORFRONTARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = verifierDef(arr[1],1);
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
			
		}
		
		else if(arr[0][0] == '9')
		{
			printf("COMMAND: tourner le bras avant gauche de %s degrees", arr[1]);
			Commandbuffer.adress = MOTORFRONTARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = verifierDef(arr[1],1);
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'a')
		{
			printf("COMMAND: tourner le bras arriere droit de %s degree", arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = verifierDef(arr[1],1);
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'b')
		{
			printf("COMMAND: tourner le bras arriere gauche de %s degree", arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = verifierDef(arr[1],1);
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'c')
		{
			printf("COMMAND: changer la vitesse avant droit a %s", arr[1]);
			Commandbuffer.adress = MOTORFRONTARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = verifierDef(arr[1],1);
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'd')
		{
			printf("COMMAND: changer la vitesse avant gauche a %s", arr[1]);
			Commandbuffer.adress = MOTORFRONTARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = verifierDef(arr[1],1);
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'e')
		{
			printf("COMMAND: changer la vitesse arrire droit a %s", arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = verifierDef(arr[1],1);
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'f')
		{
			printf("COMMAND: changer la vitesse arrire gauche a %s",arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = verifierDef(arr[1],1);
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'g')
		{
			printf("COMMAND: changer l'acceleration avant droit a %s",arr[1]);
			Commandbuffer.adress = MOTORFRONTARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = verifierDef(arr[1],1);
		}
		
		else if(arr[0][0] == 'h')
		{
			printf("COMMAND: changer l'acceleration avant gauche a %s",arr[1]);	
			Commandbuffer.adress = MOTORFRONTARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = verifierDef(arr[1],1);
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'i')
		{
			printf("COMMAND: changer l'acceleration arriere droit a %s", arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = verifierDef(arr[1],1);
		}
		
		else if(arr[0][0] == 'j')
		{
			printf("COMMAND: changer l'acceleration arriere gauche a %s",arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = verifierDef(arr[1],1);
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'p')
		{
			printf("COMMAND: Arret du moteur",arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0xffff;
			Commandbuffer.PositionRight = 0xffff;
			Commandbuffer.SpeedLeft = 0xffff;
			Commandbuffer.SpeedRight = 0xffff;
			Commandbuffer.AccelerationLeft = 0xffff;
			Commandbuffer.AccelerationRight = 0xffff;
		}
		
		else if(arr[0][0] == 'p')
		{
			printf("COMMAND: Parametre par defaut",arr[1]);
			Commandbuffer.adress = MOTORREARARM;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = 0;
			Commandbuffer.PositionRight = 0;
			Commandbuffer.SpeedLeft = 0;
			Commandbuffer.SpeedRight = 0;
			Commandbuffer.AccelerationLeft = 0;
			Commandbuffer.AccelerationRight = 0;
		}
		
		else if(arr[0][0] == 'r')
		{
			printf("Moteur Frontal:\nPositionLeft:%s\nPositionRight:%s\nSpeedLeft:%s\nSpeedRight:%s\nAccelerationLeft:%s\nAccelerationRight:%s\n",
		    arr[1], arr[4], arr[2], arr[5],arr[3],arr[6]);
			Commandbuffer.adress = MOTORFRONTDRIVE;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = verifierDef(arr[1],1);
			Commandbuffer.PositionRight = verifierDef(arr[4],1);
			Commandbuffer.SpeedLeft = verifierDef(arr[2],1);
			Commandbuffer.SpeedRight = verifierDef(arr[5],1);
			Commandbuffer.AccelerationLeft = verifierDef(arr[3],1);
			Commandbuffer.AccelerationRight = verifierDef(arr[6],1);
		}
		
		else if(arr[0][0] == 's')
		{
			printf("Moteur Arriere:\nPositionLeft:%s\nPositionRight:%s\nSpeedLeft:%s\nSpeedRight:%s\nAccelerationLeft:%s\nAccelerationRight:%s\n",
		    arr[1], arr[4], arr[2], arr[5],arr[3],arr[6]);
			Commandbuffer.adress = MOTORFRONTDRIVE;
			Commandbuffer.category = CATEGORIEGENERAL;
			Commandbuffer.type = CONSIGNESANSREPLIQUES;
			Commandbuffer.PositionLeft = verifierDef(arr[1],1);
			Commandbuffer.PositionRight = verifierDef(arr[4],1);
			Commandbuffer.SpeedLeft = verifierDef(arr[2],1);
			Commandbuffer.SpeedRight = verifierDef(arr[5],1);
			Commandbuffer.AccelerationLeft = verifierDef(arr[3],1);
			Commandbuffer.AccelerationRight = verifierDef(arr[6],1);
		}
		else if(arr[0][0] == 'k')
		{
			printf("COMMAND: liste de commandes");	
		}
		  else
		  {
			printf("nothing");  
		  }
			printf("\n");
		
		sem_post(&buflockfull);
	}
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


int verifierDef(char* arr, int neg)
{
	if(arr[0] == "d")
	{
		return 0xffff;	
	}
	else
	{
		return (neg*atoi(arr));
	}
}

int verifierDefFloat(char* arr, int neg)
{
	if(arr[0] == "d")
	{
		return 0xffff;	
	}
	else
	{
		return (neg*atoi(arr));
	}
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


