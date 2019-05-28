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

int getConection(struct conectionInfo *);
void getSome(struct conectionInfo *, char*, int);
struct conectionInfo * setInfo(char*, char*, char*, char*);
int login_add_User(struct conectionInfo *, char* , char*, char*);

#endif
