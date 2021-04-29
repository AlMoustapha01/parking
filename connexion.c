/** 
 *@file gestionVoiture.c
 *@brief Projet GESTION PARKING
 *@author AHIBA ANOKOUA STEPHANIE
 *@version 1.0
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <mysql/mysql.h> 
#include <time.h>
#include "structure.h"

MYSQL mysql;

/** 
 *@brief Fonction connexion permettant de se connecter a la base de données
*/
int connexion(){
	
	mysql_init(&mysql);
	 mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

	 if(mysql_real_connect(&mysql,"localhost","root","1234","PARKING",0,NULL,0))
	 	return 1;
    else
 		return 0;
}