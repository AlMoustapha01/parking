/** 
 *@file parking.c
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
char retour;




/** 
 *@brief Fonction place_disponible permettant d'afficher le nombre de place disponible
*/
void place_disponible(){
    int conn = connexion();
    char requete[100];
    MYSQL_RES *result = NULL ;
    if (conn == 1)
        {
	      //preparation de la requete pour recuperer la liste des places disponible
	       sprintf(requete,"select id_place ,type from place where etat=0");

            if (mysql_query(&mysql,requete) != 0)// si la requete ne s'execute pas
                 printf("%s\n", mysql_error(&mysql) );

	        else//si la requete s'execute
	             {
	               result = mysql_store_result(&mysql);
	               long nombre_de_place_disponible = mysql_num_rows(result); // on recupere le nombre de lignes retournées
	                  if (nombre_de_place_disponible ==0){
	                      printf("Aucune place disponible !!!!\n");
	    
	                    }

	                  else
	                      printf("nombre est :   %ld\n",nombre_de_place_disponible );
       
                 }

           scanf("%c",&retour);
           retourner();
        }
    else
        {
	       printf("impossible de se connecter \n");

       }

}


/** 
 *@brief Fonction place_disponible permettant d'afficher le nombre de place disponible pour une type de vehicule
*/
void place_disponible_pour_une_categorie(char *type){

	 int conn = connexion();//on se connecte a la base de données 
     char requete[100];
     MYSQL_RES *result = NULL ;
     MYSQL_ROW rows = NULL;

     if (conn == 1)
           {
	           //preparation de la requete pour recuperer la liste des places disponibles
	           sprintf(requete,"select id_place ,type from place where etat = 0 and type ='%s' ",type);

                if (mysql_query(&mysql,requete) != 0){
		              printf("%s\n", mysql_error(&mysql) );
	               }
	           else{
	                   result = mysql_use_result(&mysql);
	                   printf("Liste des places disponible au parking %s\n", type );
                        while(rows=mysql_fetch_row(result)){
     	                      printf("La place de parking %s est disponible\n",rows[0] );
                          }
	
                    }

                retourner();
           }
    else{
	    printf("impossible de se connecter \n");
        }


}



/** 
 *@brief Fonction ajouter_ permettant d'ajouter une place de parking
*/

void ajouter_parking(char *type){
	int conn = connexion();
    if(conn == 1){
         
            char requete[150];
            sprintf(requete," INSERT INTO place(etat,type) VALUES (0,'%s')",type);
            	if(mysql_query(&mysql,requete) != 0)
            	  {
            	  	printf("%s\n", mysql_error(&mysql) );
            	  	
            	  }
            	else
            	  {
            	  printf("AJOUT EFFECTUER AVEC SUCCES \n");
            	   
                  }
                  
            scanf("%c",&retour);
           retourner();
            mysql_close(&mysql);
                      
    }
  else
  	printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");

}

/** 
 *@brief afficher_liste_des_stationnement permettant d'afficher la liste des vehicules stationnés
*/
void afficher_liste_des_stationnement(){

     int conn = connexion();//on se connecte a la base de données 
     char requete[100];
     MYSQL_RES *result = NULL ;
     MYSQL_ROW rows = NULL;

     if (conn == 1)
           {
               //preparation de la requete pour recuperer la liste des places disponibles
               sprintf(requete,"select id_place ,type ,matricule from client where heure_sortie is NULL ORDER BY type");

                if (mysql_query(&mysql,requete) != 0){
                      printf("%s\n", mysql_error(&mysql) );
                   }
               else{
                       result = mysql_use_result(&mysql);
                       printf("************************LISTE DES VEHICULES STATIONNÉS****************************** \n\n");
                        while(rows=mysql_fetch_row(result)){
                              printf("\n%s immatriculé %s est stationné a la place de parking %s\n",rows[1],rows[2],rows[0] );
                          }
    
                    }

               scanf("%c",&retour);
               retourner();
           }
    else{
        printf("impossible de se connecter \n");
        }

}


