#ifndef UART_H
#define UART_H

void InitUART();
void sendUART();
void receiveUART();

void sendMessageUART(int nbBytes, char *ptrMessage);

#endif
