#include "cliente.h"


struct servidor * inicializarServerStruct(char* in_ipServer, int in_puerto){
  struct servidor *pServer = malloc(sizeof(struct servidor));
  pServer->sockfd = -1; //numero del archivo de socket
  bzero(&pServer->servaddr, sizeof(&pServer->servaddr));
  pServer->servaddr.sin_family = AF_INET;
  pServer->servaddr.sin_addr.s_addr = inet_addr(in_ipServer);
  pServer->servaddr.sin_port = htons(in_puerto); //puerto del servidor
  return pServer;
}

void inicializarPuerto(struct servidor * pServer){
  pServer->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (pServer->sockfd == -1) {
    printf("No se pudo acceder al socket!!!\n");
    exit(0);
  }else
    printf("Socket creado con exito!!!\n");
}

void conectToServer(struct servidor * pServer){
    if (connect(pServer->sockfd, (struct sockaddr*)&pServer->servaddr,sizeof(pServer->servaddr)) != 0) {
        printf("Error, no se pudo conectar al servidor...\n");
        exit(0);
    }
    else
        printf("Conectado con el servidor...\n");
}

int main(){
  char* ipServer = "192.168.0.29";
  struct servidor *myServer = inicializarServerStruct(ipServer,50000);
  inicializarPuerto(myServer);
  conectToServer(myServer);
  char mensaje[100];
  getDataUser(myServer, &mensaje);
  printf("%s \n",mensaje);
  while(1){
    int optiopn = menuInicial(myServer);
    if(optiopn == 1){
      printf("Login\n");
      sendDataServerInt(myServer,&optiopn);
    }

    if(optiopn == 2){
      printf("New user\n");
      sendDataServerInt(myServer,&optiopn);
    }

    if(optiopn == 3){
      sendDataServerInt(myServer,&optiopn);
      printf("Adios!!!!\n");
      break;
    }

    sleep(1);
  }
  close(myServer->sockfd);
  //printf("Adios!!!!\n");
}

int menuInicial(struct servidor * pServer){
  char * mensaje = "╔═════════════════════════════════╗\n   Menu Inicial \n » 1-Login.\n » 2-Nuevo User.\n » 3-Salir\n╚═════════════════════════════════╝\n";
  int optionUser = 0;
  printf("%s",mensaje);
  printf("Digite la opcion: ");
  scanf("%d", &optionUser);
  getchar();
  printf("La opcion es: %d \n",optionUser);
  return optionUser;
}

void getDataUser(struct servidor * pServer, void *buffer){
  bzero((char*)buffer, sizeof(buffer)); //limpia el buffer
  //read(pServer->sockfd,buffer , sizeof(buffer));
  //size_t total = 0; /* everything received */
  //while ( 1 ) {
    //printf("Estoy aca\n");
    //ssize_t nb = recv( pServer->sockfd,buffer , 100, 0 );
    //if ( nb == -1 ) printf( "recv failed" );
    //if ( nb == 0 ) break; /* got end-of-stream */
    //if ( write( file_fd, buffer, nb ) == -1 ) err( "file write failed" );
    //total += nb;
 //}

 //char buff[10000];
 size_t len = sizeof(buffer);
 char *p = buffer;
 ssize_t n;
 while ( len > 0 && (n=recv(pServer->sockfd,p,len,0)) > 0 ) {
   p += n;
   len =- (size_t)n;
 }
 if ( len > 0 || n < 0 ) {
   // oops, something went wrong
 }

 printf("Salgo");
}

void sendDataServerStr(struct servidor * pServer, char * mensaje){
  write(pServer->sockfd, mensaje, strlen(mensaje));
}

void sendDataServerInt(struct servidor * pServer, int *mensaje){
  write(pServer->sockfd, mensaje, sizeof(mensaje));
}
/*

void menuInicial(struct clientConect * myCliente){
  char mensaje[100] = "|  Menu Inicial    |\n| 1- Login User. \n| 2-Nuevo Usuario.\n| 3-Salir.";
  while(1){
    int optionUser = 0;
    scanf("Digite la opcion: %d", &myInt);
    if(optionUser == 3)
      break;
    if(optionUser == 1)
      login();
  }
  //sendDataUser(myCliente, mensaje);
  //getDataUser(myCliente, &optionUser);
  //optionUser = optionUser - '0';
}

void login(){

}



void getDataUser(struct clientConect * myCliente, void *buffer){
  bzero(buffer, sizeof(buffer)); //limpia el buffer
  read(myCliente->connfd,buffer , sizeof(buffer));
}
*/
