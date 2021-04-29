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
//char retour;


/** 
 *@brief Fonction connexion permettant de verifier s'il existe une place libre pour un type de vehicule s'il existe il retourne l'id de place disponible sinon il retourne 0
*/

 
 int verification(char *type){
	  int conn = connexion();

        if(conn == 1){

        	char requete[500];
            MYSQL_RES *result = NULL;
            MYSQL_ROW rows = NULL;

	  if (strcmp(type,"voiture")==0)//VERIFIER SI C'EST UNE VOITURE QUI DOIT STATIONNER
	  {
	  	
	  	sprintf(requete,"select type,id_place from place where (etat = 0) AND (type = 'voiture' or type ='camion') ORDER BY  type DESC,id_place");

	  	   if (mysql_query(&mysql,requete)==0)//SI LA REQUETE  FONCTIONNE
	  	    {
	  	    	  result = mysql_use_result(&mysql);
            rows = mysql_fetch_row(result);
            if (!rows)//SI LA REQUETE NE RETOURNE RIEN
            {
            	printf("AUCUNE PLACE DISPONIBLE\n");
            	 mysql_close(&mysql);
            	return 0;
            }
            else//SI LA REQUETE RETOURNE DES DONNEES
            {
            
            	printf("PLACE DISPONIBLE EN PARKING **%s**!!!!!!! \n",rows[0]);
            	int place_dispo = atoi(rows[1]);
            	 mysql_close(&mysql);
            	return place_dispo;
            	
            }
	  	    } 
	  	    else
	  	    	printf("%s\n", mysql_error(&mysql) );



	  }
        else//SI C'EST UN CAMION OU UN ENGIN A DEUX ROUES
        {    
            char *req = "SELECT id_place,type FROM place  WHERE etat = 0 AND type ='";
            
           sprintf(requete,"%s%s'",req,type);
           mysql_query(&mysql,requete);
            result = mysql_use_result(&mysql);
            rows = mysql_fetch_row(result);
            if (!rows)
            {
            	//printf("AUCUNE PLACE DISPONIBLE\n");
            	 mysql_close(&mysql);
            	return 0;
            }
            else{
            	//printf("PLACE DISPONIBLE !!!!!!! \n");
            	int place_dispo = atoi(rows[0]);
            	 mysql_close(&mysql);
            	return place_dispo;
            	
            }
            

            
}
        }
  else
  	printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");


}


/** 
 *@brief Fonction client_existe  permettant de verifier si un vehicule est encore stationné dans  le parking il retourne 0 si le vehicule n'est pas stationné et 1 dans le cas contraire
*/


int client_existe(char *matricule){
    char requete[1000], s[100];
    MYSQL_RES *result = NULL;
    MYSQL_ROW rows =  NULL;

    int conn = connexion();
    if (conn == 1)
      {
   	    sprintf(requete,"select count(*) from client where matricule = '%s' AND heure_sortie IS NULL",matricule);
   	 
   	   if ( mysql_query(&mysql,requete)!= 0)
   	    {
   	   	  printf("%s\n", mysql_error(&mysql) );
            	  	exit(0);
   	     }

   	   else{
           result = mysql_use_result(&mysql);
           rows = mysql_fetch_row(result);
           if (!rows)
           {  
           	printf("il retourne rien \n");
           	  return 0;
           }
           else{
           	   strcpy(s,rows[0]);
           	   int nombre = atoi(s);
           	    if (nombre == 0)
                {

                  return 0;
                }
                else{
           	   //printf(" le nombre %d\n", nombre );
           	   return 1;
             }

           }
   	   }
     }
  


}

/** 
 *@brief Fonction enregistrer_client permettant d'enregistrer un client dans la base de données 
*/



int enregister_client(char *type ,char *mat,char *nom ,char *tel){

 /**On verifie s'il existe encore des places avant d'enregistrer le client **/

	int verifier = verification(type);

    int conn = connexion();
        if(conn == 1){
            /**s'il n'y a plus de place**/
            if (verifier==0)
            {
            	printf("AUCUNE PLACE DISPONIBLE\n");
            	retourner();
            	return 0;
            }
            /**s'il ya une place disponible**/
            else{
              
            	 
            	   char requete[150];
            	   /**preparation de la requete pour inserer les données du client **/
            	 
            	   sprintf(requete," INSERT INTO client(type,matricule,heure_arrive,id_place,nom_client,telephone) VALUES ('%s','%s',NOW(),%d,'%s','%s')",type,mat,verifier,nom,tel);
            	  if(mysql_query(&mysql,requete) != 0)//si la requete fonctionne pas
            	  {
            	  	printf("%s\n", mysql_error(&mysql) );
            	  	return 0;
            	  }
            	  else//si l'insertion se fait sans erreur
            	  {
            	  printf("ENREGISTREMENT EFFECTUER AVEC SUCCES \n");
            	   mise_a_jour_place(verifier,1); //on change l'etat de la place dee parking
            	   return 1;
                  }
            }

             mysql_close(&mysql);
             
             
        }
  else
  	printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");


}

/** 
 *@brief Fonction mise_a_jour_place qui permet de mettre a jour le statut d'une place de parking (1) pour occuper ou (0) pour liberer
*/


void mise_a_jour_place (int id , int statut){
	
	 int conn = connexion();
        if(conn == 1){
         
                 char requete[100];
	              sprintf(requete,"UPDATE place SET etat = %d WHERE id_place = %d ",statut,id);
            	  if(mysql_query(&mysql,requete) != 0){
            	  	printf("%s\n", mysql_error(&mysql) );
            	  	//return 0;
            	  }
            	  else{
            	  	if(statut == 1)
            	  printf("LA PLACE %d EST MAINTENANT OCCUPÉE \n",id);
            	     else
            	   printf("LA PLACE %d EST MAINTENANT LIBRE \n",id);
                  }
                   mysql_close(&mysql);
            }
            
             
        
        else
  		printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");


}

/** 
 *@brief Fonction mise_a_jour_sortie permettant d'inserer la date et l'heure de sortie d'un vehicule
*/

void mise_a_jour_heure_sortie(char *matricule){
 


   int conn = connexion();
        if(conn == 1){
         
                 char requete[100];
       
                sprintf(requete,"UPDATE client SET heure_sortie = NOW() WHERE matricule = '%s' and heure_sortie is NULL ",matricule);
                if(mysql_query(&mysql,requete) != 0){
                  printf("%s\n", mysql_error(&mysql) );
                  
                
                   mysql_close(&mysql);
            }

          }
            
       else
           printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");

}





/** 
 *@brief Fonction supprimer_client permettant de supprimer un  client dans la base de données
*/

void supprimer_client( char *matricule){
	 int conn = connexion();
	
        if(conn == 1){
         
                 char requete[100];
	              sprintf(requete,"DELETE FROM client WHERE  matricule='%s' ",matricule);
            	  if(mysql_query(&mysql,requete) != 0){
            	  	printf("%s\n", mysql_error(&mysql) );
            	  	//return 0;
            	  }
            	  else{
            	  	   retourner();
            	  	 
                  }
                  
                   mysql_close(&mysql);
            }
            
             
        
        else
  		printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");


}

