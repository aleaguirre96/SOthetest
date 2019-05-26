#include "game.h"

//190.171.112.67

void setRespUser(struct userGame *userG, int puntosSet[], int len){
  int i;
  for(i = 0; i < len; i++) userG->resp[i] = puntosSet[i];
}

struct userGame* crearUser(char* in_userName, int in_puntos, int in_resp[]){
  struct userGame *tmpUser = malloc(sizeof(struct userGame));
  tmpUser->userName = in_userName;
  tmpUser->puntos = in_puntos;
  setRespUser(tmpUser, in_resp, MAXQUEST);
  return tmpUser;
}

void setTextPregunta(struct pregunta *ptPregunta, char* preguntaTextSet[], int len){
  int i;
  for(i = 0; i < len; i++) ptPregunta->preguntaText[i] = preguntaTextSet[i];
}

void setPregunta(struct gameAct *tmpGame, struct pregunta *preg[5], int len){
  int i;
  for(i = 0; i < len; i++) tmpGame->preguntas[i] = preg[i];
}


struct pregunta* crearPregunta(int in_numeroPregunta,char* dataPregunta[]){
  struct pregunta *tmpQuest = malloc(sizeof(struct pregunta));
  tmpQuest->numeroPregunta= in_numeroPregunta;
  setTextPregunta(tmpQuest, dataPregunta, 4);
  return tmpQuest;
}

struct gameAct*  inicializarGame(struct clientConect *cliente,struct userGame *usA, struct userGame *usB, int rond, struct pregunta *preg[5]){
  struct gameAct *tmpGame = malloc(sizeof(struct gameAct));
  tmpGame->myCliente = cliente;
  tmpGame->userA = usA;
  tmpGame->userB = usB;
  tmpGame->ronda = rond;
  setPregunta(tmpGame,preg, 5);
  return tmpGame;
}

void startGame(struct gameAct* juegoActual){
   sendQuestions(juegoActual);
}

void sendQuestions(struct gameAct* juegoActual){
  sendDataUser(juegoActual->myCliente, "initq\n"); //se le dice que está a punto de recibir preguntas
  int i = 0;
  int respuesta = 0;
  for(i; i < 5; i++){
    int j;
    printf("Mandando pregunta %d \n", i);
    for(j = 0; j < 4; j++){
      sendDataUser(juegoActual->myCliente,juegoActual->preguntas[i]->preguntaText[j]);
      sleep(1);
    }

    while(1){
      getDataUser(juegoActual->myCliente,&respuesta);
      printf("Valor respuesta en ciclo: %d \n", respuesta);
      if(respuesta != 0){
        printf("Es distinto de 0 %d El valor de i es: %d \n",respuesta, i);
        break;
      }
      sleep(1);
    }

    respuesta = respuesta - '0';
    printf("Respuesta User: %d  contador: %d \n", respuesta, i);
  }

  sendDataUser(juegoActual->myCliente, "endq \n"); //se le dice que ya no hay más preguntas
  printf("Finaliza turno...\n");
}
