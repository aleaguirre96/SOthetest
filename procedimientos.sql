USE JUEGODB;

DELIMITER //
CREATE PROCEDURE create_user(IN pUSERNAME VARCHAR(50),IN pPASS VARCHAR(50))
	BEGIN
		INSERT INTO JUGADOR(USERNAME, PASS)
		VALUES(pUSERNAME, pPASS);
	END//
DELIMITER ;

CALL create_user("joseA","1234");
CALL create_user("monseA","1234");
select * from JUGADOR;

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


call create_pregunta("Cafe o Te","Cafe, siempre","Te, obviamente","Ninguna");

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

call update_pregunta(1, 0,"Cafe o Te???");
call update_pregunta(1, 1,"Cafe, siempre.");
call update_pregunta(1, 2,"Te, obviamente.");
call update_pregunta(1, 3,"Ninguna.");
select * from PREGUNTA;
