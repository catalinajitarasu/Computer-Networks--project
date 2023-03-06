-- MariaDB dump 10.19  Distrib 10.6.11-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: MagazinOnline
-- ------------------------------------------------------
-- Server version	10.6.11-MariaDB-1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Categorie`
--

DROP TABLE IF EXISTS `Categorie`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Categorie` (
  `CategorieNume` varchar(20) NOT NULL,
  PRIMARY KEY (`CategorieNume`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Categorie`
--

LOCK TABLES `Categorie` WRITE;
/*!40000 ALTER TABLE `Categorie` DISABLE KEYS */;
INSERT INTO `Categorie` VALUES ('Accesorii telefoane'),('Electrocasnice mari'),('Electrocasnice mici'),('Gaming');
/*!40000 ALTER TABLE `Categorie` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `CosCump`
--

DROP TABLE IF EXISTS `CosCump`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CosCump` (
  `ProdusNume` varchar(20) DEFAULT NULL,
  `UserNume` varchar(50) DEFAULT NULL,
  KEY `ProdusNume` (`ProdusNume`),
  KEY `UserNume` (`UserNume`),
  CONSTRAINT `CosCump_ibfk_1` FOREIGN KEY (`ProdusNume`) REFERENCES `Produs` (`ProdusNume`),
  CONSTRAINT `CosCump_ibfk_2` FOREIGN KEY (`UserNume`) REFERENCES `User` (`UserNume`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `CosCump`
--

LOCK TABLES `CosCump` WRITE;
/*!40000 ALTER TABLE `CosCump` DISABLE KEYS */;
INSERT INTO `CosCump` VALUES ('Husa','ana'),('Incarcator','ana'),('Husa','ana'),('Husa','ana'),('Aragaz','ana'),('Masina rufe','ana');
/*!40000 ALTER TABLE `CosCump` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Produs`
--

DROP TABLE IF EXISTS `Produs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Produs` (
  `ProdusNume` varchar(20) NOT NULL,
  `CategoriNume` varchar(20) DEFAULT NULL,
  `ProdusPret` float DEFAULT NULL,
  `ProdusCantitate` int(5) DEFAULT NULL,
  PRIMARY KEY (`ProdusNume`),
  KEY `CategoriNume` (`CategoriNume`),
  CONSTRAINT `Produs_ibfk_1` FOREIGN KEY (`CategoriNume`) REFERENCES `Categorie` (`CategorieNume`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Produs`
--

LOCK TABLES `Produs` WRITE;
/*!40000 ALTER TABLE `Produs` DISABLE KEYS */;
INSERT INTO `Produs` VALUES ('Aragaz','Electrocasnice mari',1320,8),('Aspirator','Electrocasnice mici',650,12),('Cafetiera','Electrocasnice mici',250,15),('Consola Nintendo','Gaming',2100,12),('Fier de calcat','Electrocasnice mici',200,20),('Folie protectie','Accesorii telefoane',100,25),('Frigider','Electrocasnice mari',1500,13),('Husa','Accesorii telefoane',100,29),('Incarcator','Accesorii telefoane',70,19),('Masina rufe','Electrocasnice mari',1000,4),('Masina vase','Electrocasnice mari',700,10),('PlayStation','Gaming',3150,20),('Suport auto','Accesorii telefoane',65,10),('Xbox','Gaming',1499,12);
/*!40000 ALTER TABLE `Produs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `User`
--

DROP TABLE IF EXISTS `User`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `User` (
  `UserNume` varchar(50) NOT NULL,
  PRIMARY KEY (`UserNume`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `User`
--

LOCK TABLES `User` WRITE;
/*!40000 ALTER TABLE `User` DISABLE KEYS */;
INSERT INTO `User` VALUES ('ana'),('catalina'),('maria');
/*!40000 ALTER TABLE `User` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-01-10 20:52:05
