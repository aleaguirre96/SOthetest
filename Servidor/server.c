#include "server.h"
#include "conector.h"

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
      resolverPeticion(myCliente, optionUser); //Se atiende la peticion del user
      sleep(1);
  }
  printf("El cliene %d se ha desconectado. \n",myCliente->connfd);
  close(myCliente->connfd);
  free(myCliente);

  pthread_exit(0);

}

void resolverPeticion(struct clientConect * myCliente, int option){
  switch (option){
      case 1:
          login(myCliente);
          break;
      case 2:
          break;
      default:
          break;
  }

}

int login(struct clientConect * myCliente){
  char nombreUser[120];
  char pass[120];
  //Se espera el nombre del usuario
  getDataUserString(myCliente,&nombreUser);
  //Se espera la contrasena
  getDataUserString(myCliente,&pass);
  printf("NOmbre es: %s pass es: %s\n", nombreUser,pass );
  struct conectionInfo * myConector = setInfo("localhost","malexander","Usuario.1", "JUEGODB");
  int resp = login_add_User(myConector,nombreUser,pass,"call validate_login(?, ?);");
  printf("EL usuario : %d \n", resp);
  //se le manda la respuesta al CLIENTE
  send(myCliente->connfd, &resp, sizeof(&resp), 0 );
}

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
    total += resultado;
    //printf("Leo %d total: %d\n", resultado, total);
  }
  resultado = 0;
  //printf("Salgo de la lectura\n");
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
