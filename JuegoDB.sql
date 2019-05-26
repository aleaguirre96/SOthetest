-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: May 26, 2019 at 03:12 PM
-- Server version: 5.7.26-0ubuntu0.18.04.1
-- PHP Version: 7.2.17-0ubuntu0.18.04.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `JuegoDB`
--

-- --------------------------------------------------------

--
-- Table structure for table `Jugadores`
--

CREATE TABLE `Jugadores` (
  `id_Jugador` int(10) NOT NULL,
  `usuario` varchar(50) COLLATE utf8_spanish_ci NOT NULL,
  `pass` varchar(50) COLLATE utf8_spanish_ci NOT NULL,
  `PeticionesP` int(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Partida`
--

CREATE TABLE `Partida` (
  `id_Partida` int(10) NOT NULL,
  `JugadorA` int(10) NOT NULL,
  `JugadorB` int(10) NOT NULL,
  `Puntos` int(20) NOT NULL DEFAULT '0',
  `Ronda` int(10) NOT NULL DEFAULT '0',
  `id_PregActual` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Preguntas`
--

CREATE TABLE `Preguntas` (
  `id_Preguntas` int(10) NOT NULL,
  `Pregunta` varchar(200) COLLATE utf8_spanish_ci NOT NULL,
  `Respuesta1` varchar(100) COLLATE utf8_spanish_ci NOT NULL,
  `Respuesta2` varchar(100) COLLATE utf8_spanish_ci NOT NULL,
  `Respuesta3` varchar(100) COLLATE utf8_spanish_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Respuestas`
--

CREATE TABLE `Respuestas` (
  `id_PartidaFK` int(10) NOT NULL,
  `id_PreguntaFK` int(10) NOT NULL,
  `id_Jugador` int(10) NOT NULL,
  `Respuesta` varchar(20) COLLATE utf8_spanish_ci NOT NULL,
  `Ronda` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `Jugadores`
--
ALTER TABLE `Jugadores`
  ADD PRIMARY KEY (`id_Jugador`),
  ADD UNIQUE KEY `UNIQUE` (`usuario`,`pass`) USING BTREE;

--
-- Indexes for table `Partida`
--
ALTER TABLE `Partida`
  ADD PRIMARY KEY (`id_Partida`),
  ADD UNIQUE KEY `id_PregActual` (`id_PregActual`),
  ADD KEY `JugadorA` (`JugadorA`),
  ADD KEY `JugadorB` (`JugadorB`);

--
-- Indexes for table `Preguntas`
--
ALTER TABLE `Preguntas`
  ADD PRIMARY KEY (`id_Preguntas`);

--
-- Indexes for table `Respuestas`
--
ALTER TABLE `Respuestas`
  ADD PRIMARY KEY (`id_PartidaFK`,`id_PreguntaFK`,`id_Jugador`,`Ronda`),
  ADD KEY `id_PreguntaFK` (`id_PreguntaFK`),
  ADD KEY `id_Jugador` (`id_Jugador`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `Jugadores`
--
ALTER TABLE `Jugadores`
  MODIFY `id_Jugador` int(10) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `Partida`
--
ALTER TABLE `Partida`
  MODIFY `id_Partida` int(10) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `Preguntas`
--
ALTER TABLE `Preguntas`
  MODIFY `id_Preguntas` int(10) NOT NULL AUTO_INCREMENT;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `Partida`
--
ALTER TABLE `Partida`
  ADD CONSTRAINT `Partida_ibfk_1` FOREIGN KEY (`JugadorA`) REFERENCES `Jugadores` (`id_Jugador`),
  ADD CONSTRAINT `Partida_ibfk_2` FOREIGN KEY (`JugadorB`) REFERENCES `Jugadores` (`id_Jugador`);

--
-- Constraints for table `Respuestas`
--
ALTER TABLE `Respuestas`
  ADD CONSTRAINT `Respuestas_ibfk_1` FOREIGN KEY (`id_PartidaFK`) REFERENCES `Partida` (`id_Partida`),
  ADD CONSTRAINT `Respuestas_ibfk_2` FOREIGN KEY (`id_PreguntaFK`) REFERENCES `Preguntas` (`id_Preguntas`),
  ADD CONSTRAINT `Respuestas_ibfk_3` FOREIGN KEY (`id_Jugador`) REFERENCES `Partida` (`JugadorA`),
  ADD CONSTRAINT `Respuestas_ibfk_4` FOREIGN KEY (`id_Jugador`) REFERENCES `Partida` (`JugadorB`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
