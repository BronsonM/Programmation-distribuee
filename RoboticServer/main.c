#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

#include "CommunicationWebServer.h"
#include "msh.h"
#include "uart.h"

#include "SystemDef.h"



pthread_t taskCommunicationWebServer; //task used for the communication with the remote Web server through Node.js
pthread_t taskShell; // task used for the shell interpreter with the keyboard

// Keep Alive message for the control of the electronic peripheral slaves--> 0xFFFF, 0x0A, 0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x00
char KeepAliveMessage[16] = {0xff,0xff,KEEPALIVEMESSAGETYPE,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00};




void sendKeepAlive(); // function used to periodically send the KeepAliveMessage
void prepareMessage(); //function used to prepare the message adding the delimiter, the number of bytes field, payload part, and checksum
int CalculateChecksum(); //function used to calculate the checksum
int sendMessage(); //function used to send any type of messages


int main(int argc, char *argv[])
{

    sem_init (&buflockempty, 0,1);// semaphore used for the producer/consummer queue flaging an empty event synchronisation
    sem_init (&buflockfull,0,0);//semaphore used for the producer/consummer queue flaging the full event synchronisation
	

    pthread_create( &taskCommunicationWebServer, NULL, CommunicationWebServer, NULL );
    pthread_create( &taskShell, NULL, mainShell, NULL );
    pthread_mutex_init(&mutex, NULL);  //mutex used to take an hold on the critical area for the producer/consummer queue

    InitUART();

    while(1)
    {

		//sleep(1);
        
        
        if (sem_trywait(&buflockfull)==0)
		{ // Here we used sem_trywait to avoid blocking and be able to loop and send the KeepAlive
                
		//sem_wait(&buflockfull);
		pthread_mutex_lock(&mutex);
                prepareMessage();
        	sendMessage();
		pthread_mutex_unlock(&mutex);
        	sem_post(&buflockempty);
		}
	//could be send by an external task
        sendKeepAlive();

    }

}

//function used to prepare the message adding the delimiter, the number of bytes field, payload part, and checksum
void prepareMessage() 
{

	// Should call the function CalculateChecksum
}
/*
The very last byte of the frame is always a checksum. The checksum is calculated based on all the bytes that came before it. It’s a simple sum of all the bytes that made up the frame,used at the receiving end to check and see if there was a transmission error. The calculation is regular arithmetic,designed to be extremely efficient for computers to process.

Here’s the checksum formula, as stated in the official documentation:
• To calculate: Not including frame delimiters and length, add all
bytes, keeping only the lowest 8 bits of the result, and subtract the
result from 0xFF.
• To verify: Add all bytes (include checksum, but not the delimiter
and length bytes). If the checksum is correct, the sum will equal
0xFF.
*/
int CalculateChecksum(){

	return 0;
}

int sendMessage(){

    if (Commandbuffer.adress == MOTORFRONTDRIVE||Commandbuffer.adress == MOTORREARDRIVE)
	printf("Motor front drive\n");
	if (Commandbuffer.type == CONSIGNESANSREPLIQUES){
		/*Here instead of sending the Commandbuffer directly, we should send another message including that same
		Commandbuffer, but with extra fields like the delimiter, high byte for number of bytes, low byte for number
		of bytes and the checksum*/
		sendMessageUART(16, &Commandbuffer);
	}
return;
    }

// That function has to be called periodically to make sure the communication between the Raspberry PI and every electronic slaves. If the slaves do not receive the KeepAlive message in an apropriate timeframe, they stop any mechanical operation as an emergency.

void sendKeepAlive(){

    	
        sendMessageUART(16, &KeepAliveMessage[0]);
return;
    }

