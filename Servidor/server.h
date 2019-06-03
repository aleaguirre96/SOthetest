#ifndef _server_h
#define _server_h

#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "conector.h"

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
int getDataUserString(struct clientConect *, void*);
int getDataUserInt(struct clientConect *, void*);
void resolverPeticionMenuPrin(struct clientConect * myCliente, int option);
int login(struct clientConect * myCliente);
void userMenu(struct clientConect * myCliente);
void resolverPeticionMenuUser(struct clientConect * myCliente, int option);
void newGame(struct clientConect * myCliente);
void resolverPeticionMenuNewGame(struct clientConect * myCliente, int option);
void sendUsers(struct clientConect * myCliente);
int newUser(struct clientConect * myCliente);
void startNewGame(struct clientConect * myCliente);
void sendQuest(struct clientConect * myCliente, struct pregunta *arrayPreg[5], int  resp[5]);
void setNGameDataBase(struct clientConect * myCliente, struct conectionInfo * Myconector, struct pregunta *arrayPreg[5], int resp[5],int idPartida);

void partidasUser(struct clientConect * myCliente);
void resolverPeticionMenuPartidas(struct clientConect * myCliente, int option);
void sendPartidas(struct clientConect * myCliente);

#endif
