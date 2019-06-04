#!/usr/bin/env python3

import socket
titulo = "   ╔═════════════════════════════════╗\n═══╣▓▓▓▒▒▒▒░░░░░ THE TEST ░░░░▒▒▒▒▓▓▓╠═══\n   ╚═════════════════════════════════╝\n"
menuInicial = "╔═════════════════════════════════╗\n   Menu Inicial \n » 1-Login.\n » 2-Nuevo User.\n » 3-Salir\n╚═════════════════════════════════╝\n"
menuUser = "╔═════════════════════════════════╗\n   Menu User \n » 1-New Game. \n » 2-Continuar Partida\n » 3-Salir\n╚═════════════════════════════════╝\n"
menuNewGame = "╔═════════════════════════════════╗\n   Menu NewGame \n » 1-Listar Usuarios. \n » 2-Empezar a Jugar\n » 3-Salir\n╚═════════════════════════════════╝\n"
menJuego = "▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n▓▓▓▒▒░░░ THE TEST "
menJugadores = "▓╔═════════════════════════════════════════════════════════════════════════\n▓╠Los jugadores para una nueva partida son: \n▓╚═╗"
menuContinuarP = "╔═════════════════════════════════╗\n   Menu Continuar Partida \n » 1-Listar mis Partidas. \n » 2-Jugar mi turno\n » 3-Salir\n╚═════════════════════════════════╝\n"

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
    myServer = conect('ebro.ec.tec.ac.cr',50000)#'192.168.0.29', 50000)#' #
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
        partidasUser(server,name)
    else:
        server.send('3'.encode('utf-8'))#1.newGame 1.1 Get Users
# ////////////////////////////////////////////////////////////////////////////////////////////////
#2.Partidas 2.1Get Partidas 2.Continuar Partida 3.Salir
def partidasUser(server, name):
    server.send('2'.encode('utf-8')) #le avisamos al servidor que la opcion del usuer es 1
    option = 0
    listaUse = []
    while option != 3:
        option = menuPrint(server,menuContinuarP)
        listaUse = peticionPartidas(option, server, name, listaUse)

def peticionPartidas(option, server, name, listaUse):
    if(option == 1):
        ##printPartidas(server, name)
        server.send('1'.encode('utf-8'))
        mysend(name, server)
    elif(option == 2):
        ##continuarPartida(server, name, listaUse)
        server.send('2'.encode('utf-8'))
    else:
        server.send('3'.encode('utf-8'))#1.newGame 1.1 Get Users
    return listaUse



# ////////////////////////////////////////////////////////////////////////////////////////////////
#1.newGame 1.1Get User 2.Start Game 3.Salir
def newGame(server, name):
    server.send('1'.encode('utf-8')) #le avisamos al servidor que la opcion del usuer es 1
    option = 0
    listaUse = []
    while option != 3:
        option = menuPrint(server,menuNewGame)
        listaUse = peticionNewGame(option, server, name, listaUse)

def peticionNewGame(option, server, name, listaUse):
    if(option == 1):
        listaUse = []
        listaUse = printJuegadores(server, name)
    elif(option == 2):
        comenzarPartida(server, name, listaUse)
    else:
        server.send('3'.encode('utf-8'))#1.newGame 1.1 Get Users
    return listaUse

def comenzarPartida(server, name, listaUse):
    if(listaUse != []):
        server.send('2'.encode('utf-8'))#1.newGame 1.2 Get UsersComenzarPartida
        #mandar mi nombre
        mysend(name, server)
        #mandar el nombre del userB
        userPosB = int(input ("▓╠» Digite el id del User:"))
        mysend(str(listaUse[userPosB]), server)
        startNewGame(server)
    else:
        print("▓╠» Cargue los usuarios primero")

def startNewGame(server):
    data = server.recv(120).decode('utf-8', 'ignore')#
    print(menJuego)
    print(repr(data))
    preg = 0
    resp = 0
    while data != "eq": #dado el caso que
        if(data == "pr"):#la proxima es el texto de pregunta
            preg =preg+1
            resp = 0
            data = server.recv(120).decode('utf-8', 'ignore')
            print("▓▒═╦══════════════════════════════════════════════════════════════════════════")
            print("▓▒╔╝Preg"+ str(preg)+": "+str(repr(data)))
        elif(data == "ru"):
            userResp = int(input("▓▒╚══════■ Mi respuesta es:"))
            server.send(str(userResp).encode('utf-8'))#1.newGame 1.1 Get Users
        else:
            resp = resp +1
            print("▓▒╠══»Resp"+str(resp)+": "+str(repr(data)))
        data = server.recv(120).decode('utf-8', 'ignore')
    #if(data == "sg"):
    #    print("▓╠»Partida Guardada, espera tu turno....")
    #else:
    #    print("▓╠»No se pudo guardar la partida.")




def printJuegadores(server, nameUser):
    server.send('1'.encode('utf-8'))#1.newGame 1.1 Get Users
    validate = server.recv(120)#Se espera la respuesta de cuantos usuarios hay
    num = int.from_bytes(validate[0:1],byteorder='big')
    return printJugadores_aux(server,nameUser,num)

def printJugadores_aux(server,nameUser,lenUsers):
    lista = []
    print(menJugadores)
    data = server.recv(120).decode()
    while data != "ec":
        lista.append(data)
        data = server.recv(120).decode()

    cont = 0
    for user in lista:
        if(user != nameUser):
            print("▓▒▒╠»%d» %s" %(cont,user))
        cont=cont + 1

    return lista


# ////////////////////////////////////////////////////////////////////////////////////////////////




def menuPrint(server, mensaje):
  print(mensaje)
  optionUser = int(input ("▓╠» Digite una opcion:"))
  return optionUser

main()
