#ifndef SYSTEMDEF_H
#define SYSTEMDEF_H

#include <pthread.h>
#include <semaphore.h>

#define DEBUG 0

//Message types

#define CATEGORIEGENERAL 0x0
#define CATEGORIEACTION 0x1

//Category types
#define KEEPALIVEMESSAGETYPE 0x0a
#define CONSIGNESANSREPLIQUES 0x1	// use to action a peripheral without reply
#define REQUETE 0x2					// use to get the status of a peripheral
#define CONSIGNEAVECREPLIQUES 0x3	// use to action a peripheral with a reply

//Adress types
#define BROADCASTADRESS 0xffff
#define POLLINGADRESS 0x0
#define MOTORFRONTDRIVE 0x10
#define MOTORREARDRIVE 0x11
#define MOTORFRONTARM 0x12
#define MOTORREARARM 0x13
#define ACCESSOIRES 0x14
#define CAMERA 0x15
#define GPS 0x16
#define SONARS 0x17



sem_t buflockempty;
sem_t buflockfull;
pthread_mutex_t mutex;

struct{ //Structure used for building messages

	int adress;
	char category;
	char type;	

	int PositionLeft;
	int SpeedLeft;
	int AccelerationLeft;

	int PositionRight;
	int SpeedRight;
	int AccelerationRight;
	
	float panAngle;
	float tiltAngle;
	float zoomLevel;
	int camON;
	
}Commandbuffer;


/*struct{ //Structure used for building messages

	int adress;
	char category;
	char type;	

	int PositionFrontLeft;
	int PositionFrontRight;
	int PositionRearLeft;
	int PositionRearRight;

	int SpeedFrontLeft;
	int SpeedFrontRight;
	int SpeedRearLeft;
	int SpeedRearRight;

	int AccelerationFrontLeft;
	int AccelerationFrontRight;
	int AccelerationRearLeft;
	int AccelerationRearRight;

	
}Commandbuffer;*/

#endif
