#include "server.h"
#include "game.h"
#include "conector.h"
//Para ejecutar el programa se usa el comando
//gcc -Wall -fno-stack-protector game.c server.c main.c -o main -lpthread $(mysql_config --cflags) conector.c $(mysql_config --libs)


int main()
{

  runServer("",50000,10);


  while(1){ //Aqui de llamarse la funcion menu del servidor
    sleep(1);
    printf(" [s] \n");
  }

  return 0;
}
