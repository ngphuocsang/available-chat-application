-- MySQL dump 10.13  Distrib 5.7.31, for Linux (x86_64)
--
-- Host: 192.168.122.202    Database: FOR_TESTING
-- ------------------------------------------------------
-- Server version	5.7.31-0ubuntu0.18.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Account`
--

DROP TABLE IF EXISTS `Account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Account` (
  `USER_ID` int(11) NOT NULL AUTO_INCREMENT,
  `USERNAME` varchar(16) NOT NULL,
  `Password` varchar(9) NOT NULL,
  `ROLES` varchar(1) NOT NULL,
  `AUTHEN_KEY` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`USER_ID`),
  UNIQUE KEY `USERNAME` (`USERNAME`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Account`
--

LOCK TABLES `Account` WRITE;
/*!40000 ALTER TABLE `Account` DISABLE KEYS */;
INSERT INTO `Account` VALUES (1,'admin','000000000','0','2fa86d5a4ebd95e2da131c373adc786eb724ebbd'),(2,'khuong','123456789','1',NULL),(3,'dat','123456789','1',NULL),(4,'nguyen','123456789','1',NULL),(5,'bao','123456789','1', NULL),(6,'hai','123456789','1',NULL);
/*!40000 ALTER TABLE `Account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chat_room`
--

DROP TABLE IF EXISTS `chat_room`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chat_room` (
  `ROOM_ID` int(11) NOT NULL AUTO_INCREMENT,
  `message_history` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ROOM_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=91 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chat_room`
--

LOCK TABLES `chat_room` WRITE;
/*!40000 ALTER TABLE `chat_room` DISABLE KEYS */;
INSERT INTO `chat_room` VALUES (1,'./chat_history/1.txt'),(80,'./chat_history/80.txt'),(90,'./chat_history/90.txt');
/*!40000 ALTER TABLE `chat_room` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chat_room_account`
--

DROP TABLE IF EXISTS `chat_room_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chat_room_account` (
  `ROOM_ID` int(11) NOT NULL,
  `ACC_USERNAME` varchar(16) NOT NULL,
  PRIMARY KEY (`ROOM_ID`,`ACC_USERNAME`),
  KEY `ACC_USERNAME` (`ACC_USERNAME`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chat_room_account`
--

LOCK TABLES `chat_room_account` WRITE;
/*!40000 ALTER TABLE `chat_room_account` DISABLE KEYS */;
INSERT INTO `chat_room_account` VALUES (90,'bao'),(90,'hoang');
/*!40000 ALTER TABLE `chat_room_account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_chat`
--

DROP TABLE IF EXISTS `group_chat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_chat` (
  `GROUP_ID` int(11) NOT NULL AUTO_INCREMENT,
  `GROUP_NAME` varchar(32) NOT NULL,
  `ACCESSIBLE_MODIFIER` varchar(1) NOT NULL,
  `OWNER` varchar(16) NOT NULL,
  `PASSWORD` varchar(4) DEFAULT NULL,
  `message_history` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`GROUP_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_chat`
--

LOCK TABLES `group_chat` WRITE;
/*!40000 ALTER TABLE `group_chat` DISABLE KEYS */;
INSERT INTO `group_chat` VALUES (1,'hoi diet quy','0','khuong',NULL,'./group_chat_history/1.txt'),(2,'hoi ban tron','0','khuong',NULL,'./group_chat_history/2.txt'),(3,'Sword master of Shimosa','0','bao',NULL,'./group_chat_history/3.txt'),(4,'Pirate land Orleans','0','bao',NULL,'./group_chat_history/4.txt'),(5,'Gate of Chaldea','0','nguyen',NULL,'./group_chat_history/5.txt'),(6,'Decov25','0','Dat',NULL,'./group_chat_history/6.txt'),(7,'Falling Temple SOLOMON','1','Dat','1234','./group_chat_history/7.txt'),(8,'Khuong Wibu trua','1','Dat','1234','./group_chat_history/8.txt'),(9,'giao doan Eris','1','khuong','1234','./group_chat_history/9.txt'),(10,'Mystery of Shinjuku','1','hoang','1234','./group_chat_history/10.txt'),(11,'Axis party','1','khuong','1234','./group_chat_history/11.txt'),(12,'MeepEmpire','1','bao','1234','./group_chat_history/12.txt'),(16,'huhu','1','hoang','666','./group_chat_history/16.txt'),(19,'grPrivatete','1','sang','1239','./group_chat_history/19.txt'),(20,'gr Public','0','sang',NULL,'./group_chat_history/20.txt'),(22,'testfr','1','hoang1','123','./group_chat_history/22.txt'),(23,'adminTest','0','sang1',NULL,'./group_chat_history/23.txt');
/*!40000 ALTER TABLE `group_chat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_chat_account`
--

DROP TABLE IF EXISTS `group_chat_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_chat_account` (
  `GROUP_ID` int(11) NOT NULL,
  `ACC_USERNAME` varchar(16) NOT NULL,
  PRIMARY KEY (`GROUP_ID`,`ACC_USERNAME`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_chat_account`
--

LOCK TABLES `group_chat_account` WRITE;
/*!40000 ALTER TABLE `group_chat_account` DISABLE KEYS */;
INSERT INTO `group_chat_account` VALUES (1,'hai'),(1,'hoang'),(2,'bao'),(2,'hoang'),(3,'hoang'),(4,'hoang'),(5,'hoang'),(6,'Dat'),(7,'bao'),(7,'dat'),(7,'hoang'),(7,'khuong'),(7,'nguyen'),(7,'sang'),(8,'hoang'),(8,'Sayonara'),(9,'hai'),(10,'hai'),(10,'hoang'),(10,'khuong'),(10,'nguyen'),(10,'SevenEleven'),(11,'bao'),(11,'hoang'),(11,'SevenEleven'),(12,'bao'),(16,'decov25'),(16,'meep'),(16,'meep1'),(19,'dat1'),(19,'hai_chua'),(19,'meep1'),(19,'sang'),(19,'test1'),(20,'hai1'),(20,'hai_chua'),(20,'khuong1'),(20,'meep1'),(20,'sang'),(20,'sang1'),(20,'test'),(20,'test1'),(22,'hoang1'),(23,'sang1');
/*!40000 ALTER TABLE `group_chat_account` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-09-17  7:58:26