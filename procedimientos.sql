USE JUEGODB;

DELIMITER //
CREATE PROCEDURE create_user(IN pUSERNAME VARCHAR(50),IN pPASS VARCHAR(50))
	BEGIN
		INSERT INTO JUGADOR(USERNAME, PASS)
		VALUES(pUSERNAME, pPASS);
	END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE get_users()
	BEGIN
		select * from JUGADOR;
	END//
DELIMITER ;

call get_users();
#DELETE FROM JUGADOR WHERE ID_JUGADOR = 9;

DELIMITER //
CREATE PROCEDURE validate_login(IN pUSERNAME VARCHAR(50),IN pPASS VARCHAR(50))
	BEGIN
		DECLARE qEncontrado INT;
		SELECT COUNT(ID_JUGADOR)
		INTO qEncontrado
		FROM JUGADOR
		WHERE USERNAME= pUSERNAME AND PASS = pPASS;

		IF(qEncontrado = 0) THEN
           SIGNAL SQLSTATE '45000'
           SET MESSAGE_TEXT = 'Contrasena o clave no valida';
		END IF;
	END//
DELIMITER ;


DELIMITER //
CREATE PROCEDURE getId(IN pUSERNAME VARCHAR(50),OUT IDUSER INT)
	BEGIN
		SELECT ID_JUGADOR
		INTO IDUSER
		FROM JUGADOR
		WHERE USERNAME= pUSERNAME;
	END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE create_partida(IN pUSERNAMEA VARCHAR(50), IN pUSERNAMEB VARCHAR(50))
    BEGIN
        DECLARE pidA INT;DECLARE pidB INT;
        call getId(pUSERNAMEA, pidA);
        call getId(pUSERNAMEB, pidB);
		INSERT INTO PARTIDA(ID_JUGADORA, ID_JUGADORB, PUNTAJE, FINALIZADA)
		VALUES(pidA, pidB, 0, 0);
        
        SELECT LAST_INSERT_ID();
	END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE setResPrRonda(IN idpartida INT, IN idRonda INT,IN jugador INT,  
IN idP INT,IN resp INT)
	BEGIN
    if (jugador = 1) then
      INSERT INTO RESPUESTA(ID_PARTIDA, ID_RONDA, ID_PREGUNTA, JUGADORA_RESP, JUGADORB_RESP)
	  VALUES(idpartida, idRonda,idP,resp,0);
	else 
	  INSERT INTO RESPUESTA(ID_PARTIDA, ID_RONDA, ID_PREGUNTA, JUGADORA_RESP, JUGADORB_RESP)
	  VALUES(idpartida, idRonda,idP,0,resp);
    end if;
	END //
DELIMITER ;

DELIMITER //
CREATE PROCEDURE setRonda(IN idpartida INT,IN numRonda INT ,IN jugador INT,  
IN idP1 INT,IN idP2 INT, IN idP3 INT, IN idP4 INT, IN idP5 INT,
IN r1 INT, IN r2 INT, IN r3 INT, IN r4 INT, IN r5 INT)
	BEGIN
      DECLARE idRonda INT; DECLARE turnAct INT;
      if(jugador = 1 ) then
         SET turnAct = 2;
	  else
         SET turnAct = 1;
      end if;
      
      INSERT INTO  RONDA(ID_PARTIDA, RONDA, TURNO_ACT)
	  VALUES(idpartida, numRonda, turnAct);
      SET idRonda = LAST_INSERT_ID(); 
      
      call setResPrRonda(idpartida, idRonda, jugador, idP1,r1);
      call setResPrRonda(idpartida, idRonda, jugador, idP2,r2);
      call setResPrRonda(idpartida, idRonda, jugador, idP3,r3);
      call setResPrRonda(idpartida, idRonda, jugador, idP4,r4);
      call setResPrRonda(idpartida, idRonda, jugador, idP5,r5);
	END //
DELIMITER ;
#SELECT * FROM RONDA
#call set_primerRonda(1,1,1,2,3,4,5,2,2,2,2,2)
#select * from RESPUESTA

DELIMITER //
CREATE PROCEDURE getPartidas(IN pUSERNAME VARCHAR(50))
BEGIN
  SELECT ID_PARTIDA, jb.USERNAME
  FROM  PARTIDA AS p INNER JOIN JUGADOR AS j on j.ID_JUGADOR = p.ID_JUGADORA
  INNER JOIN JUGADOR AS jb ON jb.ID_JUGADOR = p.ID_JUGADORB
  WHERE p.FINALIZADA = 0 AND j.USERNAME = pUSERNAME
  UNION
  SELECT ID_PARTIDA, jb.USERNAME
  FROM  PARTIDA AS p INNER JOIN JUGADOR AS j on j.ID_JUGADOR = p.ID_JUGADORB
  INNER JOIN JUGADOR AS jb ON jb.ID_JUGADOR = p.ID_JUGADORA
  WHERE p.FINALIZADA = 0 AND j.USERNAME = pUSERNAME;
  END //
DELIMITER ;

call getPartidas('jose\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0');

DELIMITER //
CREATE PROCEDURE partidafin(IN pidpartida INT)
	BEGIN
     UPDATE PARTIDA SET FINALIZADA = 1
     WHERE ID_PARTIDA = pidpartida;
	END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE setpuntaje(IN pidpartida INT, IN nuevopuntaje INT)
	BEGIN
     UPDATE PARTIDA SET PUNTAJE = nuevopuntaje
     WHERE ID_PARTIDA = pidpartida;
	END//
DELIMITER ;

DELIMITER //
CREATE PROCEDURE create_pregunta(IN ptext VARCHAR(200), IN prep1 VARCHAR(200),
								IN prep2 VARCHAR(200),IN prep3 VARCHAR(200))
	BEGIN
		INSERT INTO PREGUNTA(TEXTO, RESP1, RESP2, RESP3)
		VALUES(ptext, prep1, prep2, prep3);
	END//
DELIMITER ;


call create_pregunta("Cafe o Te?","Cafe, siempre","Te, obviamente","Ninguna");
call create_pregunta("Correr o Dibujar?","Dibujar","Correr","Ninguna");
call create_pregunta("Sugar?","aska","obvio","Nahh");
call create_pregunta("Agua o Refresco?","Agua, siempre","Refresco, obviamente","Ambas");
call create_pregunta("Plancha?","Siempre","Nunca, obviamente","");
call create_pregunta("Yo soy:__","el centro, siempre","normal, obviamente","Ninguna");
call create_pregunta("El tec es __.","hermoso","ya ni siento nada","");

DELIMITER //
CREATE PROCEDURE update_pregunta(IN idPreg INT, IN pos INT,IN ptext VARCHAR(200))
	BEGIN
    IF(pos = 0) THEN
       UPDATE PREGUNTA SET TEXTO = ptext WHERE ID_PREGUNTA = idPreg;
	ELSEIF (pos = 1) THEN
       UPDATE PREGUNTA SET RESP1 = ptext WHERE ID_PREGUNTA = idPreg;
    ELSEIF (pos = 2) THEN
       UPDATE PREGUNTA SET RESP2 = ptext WHERE ID_PREGUNTA = idPreg;
    ELSEIF (pos = 3) THEN
       UPDATE PREGUNTA SET RESP3 = ptext WHERE ID_PREGUNTA = idPreg;
	END IF;
	END//
DELIMITER ;


DELIMITER //
CREATE PROCEDURE getFiveQ()
	BEGIN
	SELECT * FROM PREGUNTA
	ORDER BY RAND()
	LIMIT 5;
	END//
DELIMITER ;
call getFiveQ();
