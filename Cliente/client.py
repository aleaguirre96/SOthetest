#!/usr/bin/env python3

import socket
titulo = "   ╔═════════════════════════════════╗\n═══╣▓▓▓▒▒▒▒░░░░░ THE TEST ░░░░▒▒▒▒▓▓▓╠═══\n   ╚═════════════════════════════════╝\n"
menuInicial = "╔═════════════════════════════════╗\n   Menu Inicial \n » 1-Login.\n » 2-Nuevo User.\n » 3-Salir\n╚═════════════════════════════════╝\n"
menuUser = "╔═════════════════════════════════╗\n   Menu User \n » 1-New Game. \n » 2-Continuar Partida\n » 3-Salir\n╚═════════════════════════════════╝\n"
menuNewGame = "╔═════════════════════════════════╗\n   Menu NewGame \n » 1-Listar Usuarios. \n » 2-Empezar a Jugar\n » 3-Salir\n╚═════════════════════════════════╝\n"


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
    print(titulo)
    myServer = conect('192.168.0.29', 50000)
    option = 0
    data = myServer.recv(120).decode()
    print(repr(data))
    while option != 3:
        option = menuPrint(myServer, menuInicial)
        peticionInicio(option, myServer)

    print("▒╬ Hasta luego jugador...")
    myServer.close()
#//////////////////////////////////////////////////////////////////////////////////////////////
def peticionInicio(option, server):
    if(option == 1):
        print("╬╬» Login")
        login(server)
    elif(option == 2):
        print("╬╬» New User")
        newUser(server)
    else:
        server.send('3'.encode('utf-8'))#1.newGame 1.1 Get Users

def newUser(server):
    server.send('2'.encode('utf-8')) #le avisamos al servidor que la opcion del usuer es 1
    userName = str(input ("▓╠» Digite su nombre:"))
    passUser = str(input ("▓╠» Digite su contrasena:"))
    mysend(userName,server)#Se envia el nombre al serverPar
    mysend(passUser,server)#Se envia el pass al server
    validate = server.recv(120)#Se espera la respuesta
    if(validate[0]):
        print("▓╠» Usuario creado con exio %s" %(userName))
    else:
        print("▓╠» No se pudo crear el user %s" %(userName))

def login(server):
    server.send('1'.encode('utf-8')) #le avisamos al servidor que la opcion del usuer es 1
    userName = str(input ("▓╠» Digite su nombre:"))
    passUser = str(input ("▓╠» Digite su contrasena:"))
    mysend(userName,server)#Se envia el nombre al serverPar
    mysend(passUser,server)#Se envia el pass al server
    validate = server.recv(120)#Se espera la respuesta
    if(validate[0]):
        print("▓╠» Bienvenido %s" %(userName))
        sesionUser(server,userName)
    else:
        print("▓╠» No se pudo iniciar con %s" %(userName))

def sesionUser(server,name):
    option = 0
    while option != 3:
        option = menuPrint(server, menuUser)
        peticionUserSesion(option, server, name)
    print("▒╬ Hasta luego jugador %s..." %(name))

def peticionUserSesion(option, server, name):
    if(option == 1):
        print("▒╬» New Game ")
        newGame(server, name)
    elif(option == 2):
        print("▒╬» Continuar Partida ")
        server.send('2'.encode('utf-8'))
    else:
        server.send('3'.encode('utf-8'))#1.newGame 1.1 Get Users
# ////////////////////////////////////////////////////////////////////////////////////////////////
#1.newGame 1.1Get User 2.Start Game 3.Salir
def newGame(server, name):
    server.send('1'.encode('utf-8')) #le avisamos al servidor que la opcion del usuer es 1
    option = 0
    while option != 3:
        option = menuPrint(server,menuNewGame)
        peticionNewGame(option, server, name)

def peticionNewGame(option, server, name):
    if(option == 1):
        printJuegadores(server, name)
    elif(option == 2):
        comenzarPartida(server)
        print("▒╬» Comenzar Partida ")
    else:
        server.send('3'.encode('utf-8'))#1.newGame 1.1 Get Users

def comenzarPartida(server):
    server.send('2'.encode('utf-8'))#1.newGame 1.1 Get Users

def printJuegadores(server, nameUser):
    server.send('1'.encode('utf-8'))#1.newGame 1.1 Get Users
    print("▒ Los jugadores para una nueva partida son: ")
    validate = server.recv(120)#Se espera la respuesta de cuantos usuarios hay
    num = int.from_bytes(validate[0:1],byteorder='big')
    printJugadores_aux(server,num)

def printJugadores_aux(server,lenUsers):
    lista = []
    for i in range(1, lenUsers):
        data = server.recv(120).decode()
        lista.append(data)
    print(lista)


# ////////////////////////////////////////////////////////////////////////////////////////////////




def menuPrint(server, mensaje):
  print(mensaje)
  optionUser = int(input ("▓╠» Digite una opcion:"))
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
