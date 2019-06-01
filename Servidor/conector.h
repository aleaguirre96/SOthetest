#ifndef _conector_h
#define _conector_h

#include <mysql.h>
#include <stdio.h>
#include <string.h>

struct conectionInfo{
  char *server;
  char *user;
  char *password;
  char *database;
  MYSQL* connection;
  MYSQL_STMT *stmt;
};


struct pregunta{
  int  idP;
  char preguntaText1[120];//Pos 0: Texto, Pos 1 - 3: Respuestas
  char preguntaText2[120];
  char preguntaText3[120];
  char preguntaText4[120];
};

struct filaSelect{
  char* fila_result[120];
  int filas;
};

int getConection(struct conectionInfo *);
struct filaSelect* getSome(struct conectionInfo *, char*);
struct conectionInfo * setInfo(char*, char*, char*, char*);
int login_add_User(struct conectionInfo *, char* , char*, char*);
void getSomeQuestions(struct conectionInfo * Myconector, struct pregunta *pregu[5]);

#endif
