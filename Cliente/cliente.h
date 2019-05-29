#ifndef _cliente_h
#define _cliente_h
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

struct servidor {
  int sockfd; //archivo de socket
  char* ipServer; //ipDelservidor
  int puerto;  //puerto de comunicacion
  struct sockaddr_in servaddr; //Estructura de servidor de c
};
struct servidor * inicializarServerStruct(char* in_ipServer, int in_puerto);
void inicializarPuerto(struct servidor * pServer);
void conectToServer(struct servidor * pServer);
int menuInicial(struct servidor * pServer);
void sendDataServerStr(struct servidor * pServer, char * mensaje);
void sendDataServerInt(struct servidor * pServer, int* mensaje);
void getDataUser(struct servidor * pServer, void *buffer);

#endif
