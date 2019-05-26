#ifndef _game_h
#define _game_h
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "server.h"
#define MAXRONDA 4
#define MAXQUEST 5

struct userGame{ //estrucura que representa un usuario en el juego
  char* userName;
  int puntos;
  int resp[5];
};

struct pregunta{
  int numeroPregunta;
  char* preguntaText[4];//Pos 0: Texto, Pos 1 - 3: Respuestas
};

struct gameAct{
  struct clientConect *myCliente;
  struct userGame *userA;
  struct userGame *userB;
  int ronda;
  struct pregunta *preguntas[5];
};

struct pregunta* crearPregunta(int ,char* dataPregunta[]);
struct userGame* crearUser(char*, int, int in_resp[]);
void setQuestionS(struct gameAct* juegoActual);
void setRespUser(struct userGame *userG, int puntosSet[], int);
void setTextPregunta(struct pregunta *ptPregunta, char* preguntaTextSet[], int);
void setPregunta(struct gameAct *tmpGame, struct pregunta *preg[5], int);
void sendQuestions(struct gameAct* );
struct gameAct* inicializarGame(struct clientConect *,struct userGame *, struct userGame *, int, struct pregunta *preg[5]);
void startGame(struct gameAct* );

#endif
