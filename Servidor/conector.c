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

void getSome(struct conectionInfo * Myconector, char* callProce, int numColumnas){
  MYSQL_RES *resultado;
  MYSQL_ROW fila_result;
  getConection(Myconector);
  //ejecutar una llamada de procedimientos a la database
  char* procedimiento = callProce;
  int execute = mysql_real_query(Myconector->connection, procedimiento, strlen(procedimiento));
  if(execute){
    printf("Ocurrio un error al ejecutar el procedimiento");
  }else{
    resultado = mysql_store_result(Myconector->connection);
    while((fila_result = mysql_fetch_row(resultado))) {
                  printf("ID: %s  User: %s Pass: %s\n", fila_result[0], fila_result[1],fila_result[2]);
    }
    mysql_free_result(resultado);
    mysql_close(Myconector->connection);
  }
}

int login_add_User(struct conectionInfo * Myconector, char* UserName,char* Pass, char* callProce){
  getConection(Myconector);
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
        long unsigned int small_hash_len = strlen(input_data[cont]);
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


/*int main() {
  struct conectionInfo * myConector = setInfo("localhost","malexander","Usuario.1", "JUEGODB");
  getSome(myConector,"call get_users();",3);
  int resp = login_add_User(myConector,"aguirre","1234","call create_user(?, ?);");
  printf("Retorna nuevo user: %d\n", resp);
  int resp2 =login_add_User(myConector, "aguirre", "1234","call validate_login(?, ?);");
  printf("Retorna login user: %d\n", resp2);
  return 0;
}*/
