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
#define TAILLE_REQUETE 1000;


MYSQL mysql;
char retour;





/** 
 *@brief Fonction duree_heure qui renvoie le nombre d'heure de stationnement d'un engin grace a son matricule 
*/

 long duree_heure(char *matricule){
  char requete[100];
  int heures,minutes,secondes;
  long nombre_heure;
  MYSQL_RES *result = NULL;
  MYSQL_ROW rows = NULL;
    int conn = connexion();
        if(conn == 1){
         
            	 
            	   char requete[100];
            	   /**Requete pour recuperer la durée entre l'heure d'arrivée et l'heure actuelle"**/
                 char *req ="select TIMEDIFF(NOW(), heure_arrive) as duree FROM client WHERE matricule = '";
               
                 sprintf(requete,"%s%s'",req,matricule);
            	  
            	  if(mysql_query(&mysql,requete) != 0)
            	  	printf("%s\n", mysql_error(&mysql) );
            	  
            	  else{
            	  	result = mysql_use_result(&mysql);
           		     rows = mysql_fetch_row(result);
           		     if (rows)
           		     {
           		     	char duree[50];
           		strcpy(duree,rows[0]);
           		sscanf(duree,"%d:%d:%d",&heures,&minutes,&secondes);
           		nombre_heure =heures;
                if (minutes>0)
                {
                	nombre_heure = nombre_heure+1;
                	return nombre_heure;
                	exit(0);
                }
           		else
           			return nombre_heure;
           		     }
                  
                     else{
                     	printf("VEUILLEZ ENTRER UN MATRICULE VALIDE \n");
                     	 sortie_parking();
                     }
            	  }
           

            
             mysql_close(&mysql);
             
        }
  else
  	printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");
  
 }


/** 
 *@brief Fonction calcul_montant permettant de calculer le prix du  stationnement d'un engin 
*/

long calcul_montant(char *matricule ,char *type){
	long prix , ajout ,jour,duree;
	ajout=0;
 
     duree = duree_heure(matricule);
     printf(" duree %ld \n",duree );
	if(strcmp(type,"camion")==0){
		if (duree >= 24)
		{
		 /* On calcule le nombre de jour*/	
          jour = duree/24;
          ajout = 30000*jour;
          /*On calcule le nombre d'heure*/
		  duree = duree % 24;

		}
     prix = 700*duree + ajout ;
     return prix;
	}
	if(strcmp(type,"voiture")){

		if (duree >= 24)
		{
          jour = duree/24;
          ajout = 20000*jour;
		  duree = duree % 24;

		}
     prix = 500*duree + ajout ;
	
		return prix;
			}
	else
	{
		if (duree >= 24)
		{
          jour = duree/24;
          ajout = 10000*jour;
		  duree = duree % 24;

		}
     prix = 300*duree + ajout ;
     return prix;
	}
}


/** 
 *@brief Fonction recuperer_id permetant de recuperer l'id de la place ou un vehicule est stationné
*/


int recuperer_id(char *matricule){
 /**Declaration des variables**/
	int id ;
	 MYSQL_RES *result = NULL;
     MYSQL_ROW rows = NULL;
	  int conn = connexion();

        if(conn == 1){
         
               char requete[100];
	              sprintf(requete,"select id_place from client where matricule ='%s'",matricule);
            	  if(mysql_query(&mysql,requete) != 0){
            	  	printf("%s\n", mysql_error(&mysql));
            	  	
            	  }
            	  else{
            	    result = mysql_use_result(&mysql);
                    rows = mysql_fetch_row(result); 
                     id = atoi(rows[0]);
				    return id;        	  
                  }
                   mysql_close(&mysql);
            }
            
             
        
        else
  		printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");
}

/** 
 *@brief Fonction mise_a_jour_facture permet de mettre a jour la facture dans la table client
*/
void mise_a_jour_facture(char *matricule,long facture){
 


   int conn = connexion();
        if(conn == 1){
         
                 char requete[1000];
       
                sprintf(requete,"UPDATE client SET facture = '%ld' WHERE matricule = '%s' and heure_sortie is NULL ",facture,matricule);
                if(mysql_query(&mysql,requete) != 0){
                  printf("%s\n", mysql_error(&mysql) );
                  
                
                   mysql_close(&mysql);
            }

          }
            
       else
           printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");

}