#include "server.h"


//Este procedimiento crea el soket de una estructura servidor
void inicializarPuerto(struct servidor * pServer){
  pServer->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (pServer->sockfd == -1) {
    printf("No se pudo acceder al socket!!!\n");
    exit(0);
  }else
    printf("Socket creado con exito!!!\n");
}

//Este procedimiento inicializa los datos de una estrutura servidor
struct servidor * inicializarServerStruct(char* in_ipServer, int in_puerto, int in_maxUsers){
  struct servidor *pServer = malloc(sizeof(struct servidor));
  pServer->sockfd = -1; //numero del archivo de socket
  bzero(&pServer->servaddr, sizeof(&pServer->servaddr));
  pServer->servaddr.sin_family = AF_INET;
  pServer->servaddr.sin_addr.s_addr = INADDR_ANY;//inet_addr(in_ipServer);
  pServer->servaddr.sin_port = htons(in_puerto); //puerto del servidor
  pServer->maxUsers = in_maxUsers; //numeroMaximo de usuarios a atender
  return pServer;
}

//Este procedimiento realiza el bind entre la ip del servidor y el socket
void bindSocketIP(struct servidor * pServer){
  int ret = (bind(pServer->sockfd, (struct sockaddr*)&pServer->servaddr, sizeof(pServer->servaddr)));
  if ( ret != 0) {
      printf("Fallo de bind de IP server y Socket... [%d].\n",ret);
      exit(0);
  }else
      printf("Bind de IPServer y Socket finalizado con exito..\n");
}

void listenUsers(struct servidor *pServer){
  if ((listen(pServer->sockfd, pServer->maxUsers)) != 0) {
      printf("El servidor falló al esperar usuarios...\n");
      exit(0);
  }else
      printf("Server esperando usuarios..\n");
}

void aceptUsers(struct servidor *pServer){ //Esto es un hilo aparte?
  while(1){
    struct clientConect * myCliente = malloc(sizeof(struct clientConect));
    int sizeClientAddr = sizeof(myCliente->clientaddr);
    myCliente->connfd = accept(pServer->sockfd, (struct sockaddr*)&myCliente->clientaddr,&sizeClientAddr); //archivo de socket para el cliente
    printf("CLIENTE %d \n",myCliente->connfd);
    if (myCliente->connfd < 0) {
        printf("Server acccept failed...\n");
        free(myCliente); //libera los recursos de myCliente
    }else{
        printf("Server acccept the client...\n"); //Aqui llama al hilo para el usuario creado
        pthread_t hiloCliente;
        pthread_create(&hiloCliente, 0,funcHiloCliente , (void *) myCliente);
        pthread_detach(hiloCliente);
    }
    sleep(1);
  }
}

void runServer(char* ipServer, int portServer, int maxUserServer){
  struct servidor *myServer = inicializarServerStruct(ipServer,portServer, maxUserServer);
  pthread_t hiloServer;
  pthread_create(&hiloServer, 0,funcHiloEscucharClientes, (void *) myServer);
  pthread_detach(hiloServer);
}

void *funcHiloEscucharClientes(void * serverPar){
  printf("Levantando Servidor con hilo aparte\n");
  struct servidor *myServer;
  if (!serverPar) pthread_exit(0); //Si es nula el puntero del cliente sale;
  myServer = (struct servidor *)serverPar; //hace el cast del parametro
  inicializarPuerto(myServer);
  bindSocketIP(myServer);
  listenUsers(myServer);
  aceptUsers(myServer);

  pthread_exit(0);
}

void *funcHiloCliente(void * clientConect){
  struct clientConect * myCliente;
  if (!clientConect) pthread_exit(0); //Si es nula el puntero del cliente sale;
  myCliente = (struct clientConect *)clientConect; //hace el cast del parametro
  char *mensaje= " » » Bienvenido al Juego The Test. « « ";
  int optionUser = 0;
  int resultado = 1;
  sendDataUser(myCliente,mensaje);
  while(resultado > 0){
      resultado = getDataUserInt(myCliente, &optionUser);
      optionUser = optionUser - 48;
      printf("User es: %d _ La opcion del user es: %d \n",myCliente->connfd,optionUser);
      if(optionUser == 3 || optionUser < 0){
        break;
      }
      resolverPeticionMenuPrin(myCliente, optionUser); //Se atiende la peticion del user
      sleep(1);
  }
  printf("El cliene %d se ha desconectado. \n",myCliente->connfd);
  close(myCliente->connfd);
  free(myCliente);

  pthread_exit(0);

}

////////////////////////////////////////////////////////////////////////////////////////////
void resolverPeticionMenuPrin(struct clientConect * myCliente, int option){
  switch (option){
      case 1:
          if(login(myCliente))
            userMenu(myCliente);//llama al que espera las respuestas de login user
          break;
      case 2:
          newUser(myCliente);
          break;
      default:
          printf("Saliendo del juego\n");
          break;
  }
}

int newUser(struct clientConect * myCliente){
  char nombreUser[120];
  char pass[120];
  //Se espera el nombre del usuario
  getDataUserString(myCliente,&nombreUser);
  //Se espera la contrasena
  getDataUserString(myCliente,&pass);
  printf("Nombre es: %s pass es: %s\n", nombreUser,pass );
  struct conectionInfo * myConector = setInfo("localhost","malexander","Usuario.1", "JUEGODB");
  int resp = login_add_User(myConector,nombreUser,pass,"call create_user(?, ?);");
  printf("EL usuario : %d \n", resp);
  //se le manda la respuesta al CLIENTE
  send(myCliente->connfd, &resp, sizeof(&resp), 0 );
  return resp;
}

int login(struct clientConect * myCliente){
  char nombreUser[120];
  char pass[120];
  //Se espera el nombre del usuario
  getDataUserString(myCliente,&nombreUser);
  //Se espera la contrasena
  getDataUserString(myCliente,&pass);
  printf("Nombre es: %s pass es: %s\n", nombreUser,pass );
  struct conectionInfo * myConector = setInfo("localhost","malexander","Usuario.1", "JUEGODB");
  int resp = login_add_User(myConector,nombreUser,pass,"call validate_login(?, ?);");
  printf("EL usuario : %d \n", resp);
  //se le manda la respuesta al CLIENTE
  send(myCliente->connfd, &resp, sizeof(&resp), 0 );
  return resp;
}
////////////////////////////////////////////////////////////////////////////////////////////
void userMenu(struct clientConect * myCliente){
  //Sabemos que puede estamos logeado
  int optionUser = 0;
  int resultado = 1;
  while(resultado > 0){
    resultado = getDataUserInt(myCliente, &optionUser);
    optionUser = optionUser - 48;
    printf("User es: %d _ La opcion del user es en USERMENU: %d \n",myCliente->connfd,optionUser);
    resolverPeticionMenuUser(myCliente,optionUser);
    if(optionUser == 3 || optionUser < 0){
      break;
    }
  }
}

void resolverPeticionMenuUser(struct clientConect * myCliente, int option){
  switch (option){
      case 1:
          newGame(myCliente);//llama al que espera las respuestas de login user
          break;
      case 2:
          printf("Opcion continuar JUego\n");//Continuar juego
          break;
      default:
          printf("Saliendo menu Principal\n");//Continuar juego
          break;
  }
}

void newGame(struct clientConect * myCliente){
  int optionUser = 0;
  int resultado = 1;
  while(resultado > 0){
    resultado = getDataUserInt(myCliente, &optionUser);
    optionUser = optionUser - 48;
    printf("User es: %d _ La opcion del user es en newGAME: %d \n",myCliente->connfd,optionUser);
    resolverPeticionMenuNewGame(myCliente,optionUser);
    if(optionUser == 3 || optionUser < 0){
      break;
    }
  }
}

void resolverPeticionMenuNewGame(struct clientConect * myCliente, int option){
  switch (option){
      case 1:
          sendUsers(myCliente);//muestra los usuarios para un juego
          break;
      case 2: //empezar Juego
          startNewGame(myCliente);
          break;
      case 3: //
          printf("Opcion Regresar Menu User\n");
          break;
      default:
          break;
  }
}

void startNewGame(struct clientConect * myCliente){//representa el turno 1 del juego
  struct conectionInfo * myConector = setInfo("localhost","malexander","Usuario.1", "JUEGODB");
  char userNameA[120];
  //Se espera el nombre del usuario
  getDataUserString(myCliente,&userNameA);
  char userNameB[120];
  //Se espera el nombre del usuario
  getDataUserString(myCliente,&userNameB);
  printf("User para nuevo Juego A: %s B: %s \n",userNameA,userNameB);
  //Pedir preguntas
  struct pregunta *arrayPreg[5];
  getSomeQuestions(myConector,arrayPreg);
  //Mandar preguntas /Pedir respuestas
  sendQuest(myCliente, arrayPreg);
  //Guardar en la base
}
void sendQuest(struct clientConect * myCliente, struct pregunta *arrayPreg[5]){
  //manda los textos de las preguntass
  int i;
  int resp[5];
  for(i = 0; i < 5; i++){
    printf("Mandando pregunta %d \n", i);
    //send(myCliente->connfd, &arrayPreg[i]->idP, sizeof(&arrayPreg[i]->idP), 0 );//id de pregunta
    sendDataUser(myCliente,"pr");
    sleep(1);
    sendDataUser(myCliente,arrayPreg[i]->preguntaText1);
    sleep(1);
    sendDataUser(myCliente,arrayPreg[i]->preguntaText2);
    sleep(1);
    sendDataUser(myCliente,arrayPreg[i]->preguntaText3);
    sleep(1);
    sendDataUser(myCliente,arrayPreg[i]->preguntaText4);
    sleep(1);
    printf("Pregunta mandada\n");
    sendDataUser(myCliente,"ru");
    getDataUserInt(myCliente, &resp[i]);
    printf("Respuesta recibida\n");
    sleep(1);
 }
 sendDataUser(myCliente,"eq");//acaba de mandar las pregntas
 printf("Resp 1:%d 2:%d 3:%d 4:%d 5:%d",resp[0],resp[1],resp[2],resp[3],resp[4]);
}

void sendUsers(struct clientConect * myCliente){
  struct conectionInfo * myConector = setInfo("localhost","malexander","Usuario.1", "JUEGODB");
  struct filaSelect* filauser = getSome(myConector,"call get_users()");
  //manda la cantidad usuarios//printf("NUm de user %d", filauser->filas);
  send(myCliente->connfd, &filauser->filas, sizeof(&filauser->filas), 0 );
  //manda los nombres de los usuarios y su id
  sleep(1);
  int i;
  for(i = 0; i < filauser->filas; i++){
    sendDataUser(myCliente,filauser->fila_result[i]);
    sleep(1);
  }
  sendDataUser(myCliente,"ec");
  printf("Mando a los usuarios\n");
}
////////////////////////////////////////////////////////////////////////////////////////////

void sendDataUser(struct clientConect * myCliente, char* mensaje){
  int len = strlen(mensaje);
  size_t total = 0;
  while ( total != len ) {
    ssize_t nb = send( myCliente->connfd, mensaje + total, len - total, 0 );
    if ( nb == -1 ) printf( "send failed \n" );
    total += nb;
  }
  printf("Se logro mandar el mensaje\n");
}

int getDataUserString(struct clientConect * myCliente, void *buffer){
  bzero(buffer, sizeof(buffer)); //limpia el buffer
  int resultado = 1; //= read(myCliente->connfd,buffer,sizeof(buffer));
  int total = 0;
  while(resultado > 0 && total < 120){
    resultado = read(myCliente->connfd,buffer,sizeof(buffer));
    buffer += resultado;
    total += resultado;//printf("Leo %d total: %d\n", resultado, total);
  }
  resultado = 0;//printf("Salgo de la lectura\n");
  return resultado;
}

int getDataUserInt(struct clientConect * myCliente, void*buffer){
  bzero(buffer, sizeof(buffer)); //limpia el buffer
  int resultado = read(myCliente->connfd,buffer,sizeof(buffer));
  return resultado;
}

/*
void jugar(struct clientConect * myCliente){ //Aquí es donde se debe de pedir los usuarios y el accesos a la base de datos
  int myA[5] = {0,0,0,0,0};
  struct userGame* tmpA = crearUser("JoseAguirre",0,myA);
  struct userGame* tmpB = crearUser("Monser",0,myA);
  char* preguntaTexttmp[4] = {"1. Esto es una prueba ", "si sirve ", "no sirve ", "ambas "};
  char* preguntaTexttmp2[4] = {"2. Esto es una prueba ", "si sirve ", "no sirve ", "ambas "};
  struct pregunta *myquest = crearPregunta(20 , preguntaTexttmp);
  struct pregunta *myquest2 = crearPregunta(20 , preguntaTexttmp2);
  struct pregunta *preguntas[5] = {myquest, myquest, myquest2, myquest, myquest2};
  struct gameAct* myGame = inicializarGame(myCliente,tmpA,tmpB,1, preguntas);
  startGame(myGame);
}
*/
