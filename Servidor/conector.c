#include "conector.h"

//https://dev.mysql.com/doc/refman/8.0/en/c-api-functions.html
struct conectionInfo * setInfo(char* pserver, char* puser, char* ppasword, char* pdatabase){
   struct conectionInfo *tmpInfo = malloc(sizeof(struct conectionInfo));
   tmpInfo->server = pserver;
   tmpInfo->user =  puser;
   tmpInfo->password = ppasword;
   tmpInfo->database = pdatabase;
   return tmpInfo;
}

int getConection(struct conectionInfo * Myconector){
  Myconector->connection = mysql_init(NULL);
  int error = 0;
  if(!mysql_real_connect(Myconector->connection,Myconector->server,Myconector->user,Myconector->password,Myconector->database,0,NULL,CLIENT_MULTI_STATEMENTS)){
    printf("No se pudo conectar a la base de datos!!!!\n");
    mysql_close(Myconector->connection);
  }else{
    error = 1;
  }
  return error;
}

struct filaSelect* getSome(struct conectionInfo * Myconector, char* callProce){
  MYSQL_RES *resultado;
  MYSQL_ROW fila_result;
  struct filaSelect* tmp = malloc(sizeof(struct filaSelect));
  getConection(Myconector);
  //ejecutar una llamada de procedimientos a la database
  char* procedimiento = callProce;
  int execute = mysql_real_query(Myconector->connection, procedimiento, strlen(procedimiento));
  if(execute){
    printf("Ocurrio un error al ejecutar el procedimiento");
  }else{
    resultado = mysql_store_result(Myconector->connection);
    tmp->filas = (int) mysql_num_rows(resultado);
    int i = 0;
  while((fila_result = mysql_fetch_row(resultado))) {
      tmp->fila_result[i] = fila_result[1];
      //printf("ID: %s  User: %s Pass: %s\n", fila_result[0], fila_result[1],fila_result[2]);
      i++;
  }
    mysql_free_result(resultado);
    mysql_close(Myconector->connection);
  }
  return tmp;
}

void getSomeQuestions(struct conectionInfo * Myconector, struct pregunta *pregu[5]){
  MYSQL_RES *resultado;
  MYSQL_ROW fila_result;
  getConection(Myconector);
  //ejecutar una llamada de procedimientos a la database
  char* procedimiento = "call getFiveQ();";
  int execute = mysql_real_query(Myconector->connection, procedimiento, strlen(procedimiento));
  if(execute){
    printf("Ocurrio un error al ejecutar el procedimiento");
  }else{
    resultado = mysql_store_result(Myconector->connection);
    int i = 0;
  while((fila_result = mysql_fetch_row(resultado))) {
      pregu[i] = malloc(sizeof(struct  pregunta));
      char* preguntaTexttmp[4] = { fila_result[1], fila_result[2], fila_result[3], fila_result[4]};
      int idP = atoi(fila_result[0]);
      pregu[i]->idP = idP;
      strcpy(pregu[i]->preguntaText1, preguntaTexttmp[0]);
      strcpy(pregu[i]->preguntaText2, preguntaTexttmp[1]);
      strcpy(pregu[i]->preguntaText3, preguntaTexttmp[2]);
      strcpy(pregu[i]->preguntaText4, preguntaTexttmp[3]);
      i++;
  }
    mysql_free_result(resultado);
    mysql_close(Myconector->connection);
  }
}

int login_add_User(struct conectionInfo * Myconector, char* UserName,char* Pass, char* callProce){
  getConection(Myconector);
  printf("ESte es el nombre %s \n", UserName);
  int status = 1;
  Myconector->stmt = mysql_stmt_init(Myconector->connection);
  if (!Myconector->stmt){
    printf("Could not initialize statement\n");
  }else{
    char* procedimiento2 = callProce;
    status = mysql_stmt_prepare(Myconector->stmt, procedimiento2, strlen(procedimiento2));
    if(!status){
      MYSQL_BIND ps_params[2];
      char* input_data[2] = {UserName, Pass};
      memset(ps_params, 0, sizeof (ps_params));
      int cont;
      for(cont = 0; cont < 2; cont++){
        long unsigned int small_hash_len = 50;
        ps_params[cont].buffer_type = MYSQL_TYPE_STRING ;
        ps_params[cont].buffer = input_data[cont];
        ps_params[cont].buffer_length = small_hash_len;
        ps_params[cont].length = &small_hash_len ;
        ps_params[cont].is_null = 0;
      }
      status = mysql_stmt_bind_param(Myconector->stmt, ps_params);
      if(!status){
        status = mysql_stmt_execute(Myconector->stmt);
      }
  }
  if(status) printf("Error: %s (errno: %d)\n",mysql_stmt_error(Myconector->stmt), mysql_stmt_errno(Myconector->stmt));
  mysql_close(Myconector->connection);
  return !status;
 }
}


int newPartidaDataBase(struct conectionInfo * Myconector,char* UserNameA,char* UserNameB){
  getConection(Myconector);
  MYSQL_RES *resultado;
  MYSQL_ROW fila_result;
  int status = 1;
  int idPartida = -1;
  Myconector->stmt = mysql_stmt_init(Myconector->connection);
  if (!Myconector->stmt){
    printf("Could not initialize statement\n");
  }else{
    char* procedimiento2 = "call create_partida(?,?);";
    status = mysql_stmt_prepare(Myconector->stmt, procedimiento2, strlen(procedimiento2));
    if(!status){
      MYSQL_BIND ps_params[3];
      char* input_data[2] = {UserNameA, UserNameB};
      memset(ps_params, 0, sizeof (ps_params));
      int cont;
      for(cont = 0; cont < 2; cont++){
        long unsigned int small_hash_len = 50;
        ps_params[cont].buffer_type = MYSQL_TYPE_STRING ;
        ps_params[cont].buffer = input_data[cont];
        ps_params[cont].buffer_length = small_hash_len;
        ps_params[cont].length = &small_hash_len ;
        ps_params[cont].is_null = 0;
      }

      status = mysql_stmt_bind_param(Myconector->stmt, ps_params);
      if(!status){
        status = mysql_stmt_execute(Myconector->stmt);
      }
  }
    if(status){
      printf("Error: %s (errno: %d)\n",mysql_stmt_error(Myconector->stmt), mysql_stmt_errno(Myconector->stmt));
    }else{
      MYSQL_BIND    bResult[1];
      memset(bResult, 0, sizeof(bResult));
      bResult[0].buffer_type= MYSQL_TYPE_LONG;
      bResult[0].buffer= (char *)&idPartida;
      mysql_stmt_bind_result(Myconector->stmt, bResult);
      mysql_stmt_store_result(Myconector->stmt);
      while(!mysql_stmt_fetch(Myconector->stmt)) {
        //printf("ID: %d \n", idPartida);
      }
      mysql_stmt_close(Myconector->stmt);
    }
    mysql_close(Myconector->connection);
  }
  return idPartida;
}

struct filaSelect* getPartidas(struct conectionInfo * Myconector, char* userName){
  getConection(Myconector);
  struct filaSelect* tmp = malloc(sizeof(struct filaSelect));
  MYSQL_RES *resultado;
  MYSQL_ROW fila_result;
  int status = 1;
  int idPartida = -1;
  char * nameV = "";
  Myconector->stmt = mysql_stmt_init(Myconector->connection);

  if (!Myconector->stmt){
    printf("Could not initialize statement\n");
  }else{
    char* procedimiento2 = "call getPartidas(?);";
    status = mysql_stmt_prepare(Myconector->stmt, procedimiento2, strlen(procedimiento2));
    if(!status){
      MYSQL_BIND ps_params[1];
      char* input_data[1] = {userName};
      memset(ps_params, 0, sizeof (ps_params));
      long unsigned int small_hash_len = 50;
      ps_params[0].buffer_type = MYSQL_TYPE_STRING ;
      ps_params[0].buffer = input_data[0];
      ps_params[0].buffer_length = small_hash_len;
      ps_params[0].length = &small_hash_len ;
      ps_params[0].is_null = 0;

      status = mysql_stmt_bind_param(Myconector->stmt, ps_params);
      if(!status){
        status = mysql_stmt_execute(Myconector->stmt);
      }
    }
    if(status){
      printf("Error: %s (errno: %d)\n",mysql_stmt_error(Myconector->stmt), mysql_stmt_errno(Myconector->stmt));
    }else{
      MYSQL_BIND    bResult[2];
      memset(bResult, 0, sizeof(bResult));
      bResult[0].buffer_type= MYSQL_TYPE_LONG;
      bResult[0].buffer= (char *)&idPartida;


      long unsigned int small_hash_len = 50;
      bResult[1].buffer_type = MYSQL_TYPE_STRING ;
      bResult[1].buffer = (char *)nameV;
      bResult[1].buffer_length = small_hash_len;
      bResult[1].length = &small_hash_len ;
      bResult[1].is_null = 0;

      //mysql_stmt_bind_result(Myconector->stmt, bResult);
      //mysql_stmt_store_result(Myconector->stmt);

      //while(!mysql_stmt_fetch(Myconector->stmt)) {
      //if(status)
        printf("Error: %s (errno: %d)\n",mysql_stmt_error(Myconector->stmt), mysql_stmt_errno(Myconector->stmt));
        //tmp->filas = idPartida;
        //strcpy(tmp->fila_resultb,nameV);
        //printf("IDPARTIDA: %d NAME: %s\n",idPartida,nameV);
      //}
      printf("Llego aca\n");
      mysql_stmt_close(Myconector->stmt);
    }
    mysql_close(Myconector->connection);
  }
  return tmp;
}

int setPrimerRonda(struct conectionInfo * Myconector,int parametros[13]){
  printf("Llego acá1\n");
  getConection(Myconector);
  MYSQL_RES *resultado;
  MYSQL_ROW fila_result;
  int status = 1;
  int idPartida = -1;
  Myconector->stmt = mysql_stmt_init(Myconector->connection);
  if (!Myconector->stmt){
    printf("Could not initialize statement\n");
  }else{
    char* procedimiento2 = "call setRonda(?,?,?,?,?,?,?,?,?,?,?,?,?);";
    status = mysql_stmt_prepare(Myconector->stmt, procedimiento2, strlen(procedimiento2));
    if(!status){
      MYSQL_BIND ps_params[13];
      memset(ps_params, 0, sizeof (ps_params));
      int cont;
      for(cont = 0; cont < 13; cont++){
        ps_params[cont].buffer_type = MYSQL_TYPE_LONG ;
        ps_params[cont].buffer = &parametros[cont];
        ps_params[cont].length = 0;
        ps_params[cont].is_null = 0;
      }

      status = mysql_stmt_bind_param(Myconector->stmt, ps_params);
      if(!status){
        status = mysql_stmt_execute(Myconector->stmt);
      }
  }
    if(status){
      printf("Error: %s (errno: %d)\n",mysql_stmt_error(Myconector->stmt), mysql_stmt_errno(Myconector->stmt));
    }
  }
  return !status;
}
