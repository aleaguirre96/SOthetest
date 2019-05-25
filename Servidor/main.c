#include "server.h"
#include "game.h"
//Para ejecutar el programa se usa el comando
//gcc game.c server.c main.c -o main -lpthread

int main()
{

  runServer("",50000,10);


  while(1){ //Aqui de llamarse la funcion menu del servidor
    sleep(1);
    printf("Aqui sigue el main\n");
  }

  return 0;
}
