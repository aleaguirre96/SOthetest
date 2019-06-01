USE JUEGODB;

DELIMITER //
CREATE PROCEDURE create_user(IN pUSERNAME VARCHAR(50),IN pPASS VARCHAR(50))
	BEGIN
		INSERT INTO JUGADOR(USERNAME, PASS)
		VALUES(pUSERNAME, pPASS);
	END//
DELIMITER ;

CALL create_user("jose","1234");
CALL create_user("mons","1234");
CALL create_user("use1","1234");
CALL create_user("use2","1234");
CALL create_user("use3","1234");
CALL create_user("use4","1234");
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

call validate_login("jsss","1234");




DELIMITER //
CREATE PROCEDURE create_partida(IN pidA INT, IN pidB INT, OUT pidPart INT)
	BEGIN
		INSERT INTO PARTIDA(ID_JUGADORA, ID_JUGADORB, PUNTAJE, FINALIZADA)
		VALUES(pidA, pidB, 0, 0);
        SET pidPart = LAST_INSERT_ID();
	END//
DELIMITER ;



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



call partidafin(1);
call setpuntaje(1, 40);
select * from PARTIDA;


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
call getFiveQ();