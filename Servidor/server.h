#ifndef _server_h
#define _server_h

#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "game.h"

struct servidor {
  int sockfd; //archivo de socket
  char* ipServer; //ipDelservidor
  int puerto;  //puerto de comunicacion
  int maxUsers;
  struct sockaddr_in servaddr; //Estructura de servidor de c
};

struct clientConect{ //estrucura que representa la coneccion de un cliente con el servidor
  int connfd;
  struct sockaddr_in clientaddr;
};

void runServer(char*, int, int);
struct servidor *  inicializarServerStruct(char*, int, int);
void inicializarPuerto(struct servidor *);
void bindSocketIP(struct servidor *);
void listenUsers(struct servidor *);
void aceptUsers(struct servidor *);
void *funcHiloCliente(void * );
void *funcHiloEscucharClientes(void * );
void sendDataUser(struct clientConect *, char*);
//void sendDataUser(struct clientConect * myCliente, char mensaje[]);
int getDataUser(struct clientConect *, void*);
void resolverPeticion(struct clientConect *);
void jugar(struct clientConect * );


#endif
