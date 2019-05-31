#!/usr/bin/env python3

import socket
menuInicial = "╔═════════════════════════════════╗\n   Menu Inicial \n » 1-Login.\n » 2-Nuevo User.\n » 3-Salir\n╚═════════════════════════════════╝\n"
menuUser = "╔═════════════════════════════════╗\n   Menu User \n » 1-New Game. \n » 2-Continuar Partida\n » 3-Salir\n╚═════════════════════════════════╝\n"


def conect(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    return s

def mysend(msg, s):
    if(len(msg) > 120):
        msg = msg[0:120]
    if(len(msg) < 120):
        msg = msg+('\0'*(120-len(msg)))
    totalsent = 0
    while totalsent < 121:
        sent = s.send(msg[totalsent:].encode('utf-8'))
        if(sent == 0):
            break
        totalsent = totalsent + sent

def main():
    myServer = conect('192.168.0.29', 50000)
    option = 0
    data = myServer.recv(120).decode()
    print(repr(data))
    while option != 3:
        option = menuPrint(myServer, menuInicial)
        peticionInicio(option, myServer)

    print("Hasta luego jugador...")
    myServer.close()

def login(server):
    server.send('1'.encode('utf-8')) #le avisamos al servidor que la opcion del usuer es 1
    print("Mando el 1")
    userName = str(input ("Digite su nombre:"))
    passUser = str(input ("Digite su contrasena:"))
    mysend(userName,server)#Se envia el nombre al serverPar
    mysend(passUser,server)#Se envia el pass al server
    validate = server.recv(120)#Se espera la respuesta
    if(validate[0]):
        print(" ╠» Bienvenido %s" %(userName))
        sesionUser(server,userName)
    else:
        print(" ╠» No se pudo iniciar con %s" %(userName))

def sesionUser(server,name):
    option = 0
    while option != 3:
        option = menuPrint(server, menuUser)
        peticionUserSesion(option, server)
    print("Hasta luego jugador %s..." %(name))

def peticionUserSesion(option, server):
    if(option == 1):
        print("» New Game")
    elif(option == 2):
        print("» Continuar Partida")

def peticionInicio(option, server):
    if(option == 1):
        print("» Login")
        login(server)
    elif(option == 2):
        print("» New User")

#def newGame(server):

def menuPrint(server, mensaje):
  print(mensaje)
  optionUser = int(input ("Digite una opcion:"))
  return optionUser

#data = s.recv(1024)
#HOST = '192.168.0.25'  # The server's hostname or IP address
#PORT = 50000        # The port used by the server

#msg ='1234567890123'
##s.sendall(msg.encode('utf-8'))
#mysend(msg,s)
#print("Mandado el mensaje")
#data = s.recv(120)
#print('Received', repr(data))
#s.send('3'.encode('utf-8'))

#s.close()
main()
