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

struct filaSelect{
  char* fila_result[120];
  int filas;
};

int getConection(struct conectionInfo *);
struct filaSelect* getSome(struct conectionInfo *, char*);
struct conectionInfo * setInfo(char*, char*, char*, char*);
int login_add_User(struct conectionInfo *, char* , char*, char*);

#endif
