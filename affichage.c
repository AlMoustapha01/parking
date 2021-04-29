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
char retour;

/** 
 *@brief Fonction retourner permettant de retourner au menu precedent
*/

 /**********fonction pour retourner au menu principal****************************/
void retourner(){
    
  printf("\npour retourner au menu principal, appuyer sur (r)\n");
  printf("pour quitter , appuyer sur (q)\n\n");
  scanf("%c",&retour);
  if (retour == 'r')
    menu();
 
  if(retour == 'q'){
    printf("bye\n");
    exit(0);
}
}


/** 
 *@brief Fonction historique permettant d'afficher l'historique des factures
*/

void historique(){

       MYSQL_RES *result;
       MYSQL_ROW rows;
       int conn = connexion();
        if(conn == 1){
         
                 char requete[1000];
       
                sprintf(requete,"select nom_client ,type ,matricule ,facture from client where facture is not NULL");
                if(mysql_query(&mysql,requete) != 0){
                  printf("%s\n", mysql_error(&mysql) ); 
            }

            else{
                       result = mysql_use_result(&mysql);
                       printf("************************HISTORIQUE DES FACTURES  ****************************** \n\n");
                        while(rows=mysql_fetch_row(result)){
                              printf(" client : %s  %s d' immatriculation %s facture : %s \n",rows[0],rows[1],rows[2],rows[3] );
                          }
    
                    }
            mysql_close(&mysql);
          }
            
       else
           printf("IMPOSSIBLE DE SE CONNECTER A LA BDD\n");
}









/** 
 *@brief Fonction entrer_parking  permettant de gerer les entrées
*/

 
 void entrer_parking(){
 	char matricule[100] ,nom[100] ,tel[100];
 	int enregistrement,choix,verifier;
 	printf("ENTRER (1) POUR LES CAMIONS\n");
	printf("ENTRER (2) POUR LES VOITURES\n");
	printf("ENTRER (3) POUR LES ENGINS A DEUX ROUES  \n");
	printf("ENTRER (0) POUR RETOURNER   \n");
	scanf("%d",&choix);
 	
    switch(choix){
    	case 1 :
    	  verifier = verification("camion");
    	  if (verifier !=0)
    	  {
             printf("entrer le matricule du camion \n");
             scanf("%s",matricule); 
             /*On verifie si ce vehicule peut etre enregistré */
             int voiture_existe = client_existe(matricule);
             if (voiture_existe == 0)
             {
                printf("entrer le nom du client \n");
             scanf("%s",nom);
             printf("entrer le numero de telephone du client \n");
             scanf("%s",tel);
             enregister_client("camion",matricule,nom,tel);
             }
            
    	  	else{
                printf("LE CAMION IMMATRICULÉ %s EST ENCORE STATIONNÉ DANS LE PARKING, VEUILLEZ REESSAYER L'ENREGISTREMENT\n" ,matricule);

            }
    	  	
    	  }
            scanf("%c",&retour);
                    retourner();
    	  break;
    	 case 2 :
    	  verifier = verification("voiture");
    	  if (verifier !=0)
    	  {
             printf("entrer le matricule de la voiture \n");
             scanf("%s",matricule); 
             /*On verifie si ce vehicule peut etre enregistré */
            int voiture_existe = client_existe(matricule);
             if (voiture_existe == 0)
             {
                printf("entrer le nom du client \n");
             scanf("%s",nom);
             printf("entrer le numero de telephone du client \n");
             scanf("%s",tel);
             enregister_client("voiture",matricule,nom,tel);
             }
            
            else{
                printf("LA VOITURE IMMATRICULÉE %s EST ENCORE STATIONNÉE DANS LE PARKING, VEUILLEZ REESSAYER L'ENREGISTREMENT\n" ,matricule);

            }
              
    	  }
            scanf("%c",&retour);
                    retourner();

    	  break;

    	   case 3 :
    	  verifier = verification("engin");
    	  if (verifier !=0)
    	  {
             printf("entrer le matricule de l'engin a deux roues \n");
             scanf("%s",matricule); 
             
             int voiture_existe = client_existe(matricule);
             if (voiture_existe == 0)
             {
                printf("entrer le nom du client \n");
             scanf("%s",nom);
             printf("entrer le numero de telephone du client \n");
             scanf("%s",tel);
             enregister_client("engin",matricule,nom,tel);
             }
            
            else{
                printf("L'ENGIN IMMATRICULÉ %s EST ENCORE STATIONNÉ DANS LE PARKING, VEUILLEZ REESSAYER L'ENREGISTREMENT\n",matricule);

            }
    	  	 
    	  	 
    	  }
           scanf("%c",&retour);
                    retourner();

    	  break;
           case 0:
		 printf("bye\n");
		 break;
		 default:
		 printf("choix indisponible veuillez entrer une reponse valide\n");
		 entrer_parking();

    }
 }

/** 
 *@brief Fonction sortie_parking permettant de gerer les sorties du parking
*/


  void sortie_parking(){
    char *matricule;
    int  reponse, id_place,choix;
    long facture;
     MYSQL_RES *result = NULL;
     MYSQL_ROW rows = NULL;
       
    printf("ENTRER (1) POUR LES CAMIONS\n");
    printf("ENTRER (2) POUR LES VOITURES\n");
    printf("ENTRER (3) POUR LES ENGINS A DEUX ROUES  \n");


    printf("ENTRER (0) POUR RETOURNER   \n");
    scanf("%d",&choix);
    
     switch(choix){
        case 1 :
         
             printf("entrer le matricule du camion \n");
             scanf("%s",matricule); 
             facture = calcul_montant(matricule,"camion");
              if(facture !=0 ){
             printf("La facture du vehicule immatriculé %s est : %ld FCFA\n",matricule,facture);

             printf("ENTRER (1) SI LE PAYEMENT A ETE EFFECTUÉ \n");
             printf("ENTRER (0) POUR SORTIR \n");    
             scanf("%d",&reponse);
             if (reponse == 1)
             {   
                mise_a_jour_facture(matricule,facture);
                 mise_a_jour_heure_sortie(matricule);
                id_place = recuperer_id(matricule);
                if (id_place != 0)
                {   

                    mise_a_jour_place(id_place ,0);
                }

             }

             else{
                printf("bye\n");
                exit(1);
               }
              
              scanf("%c",&retour);
                    retourner();
              }
          break;
        case 2:
            printf("entrer le matricule de la voiture \n");
             scanf("%s",matricule); 
             facture = calcul_montant(matricule,"voiture");
             printf("%ld\n",facture );
              if(facture !=0 ){
             printf("La facture du vehicule immatriculé %s est : %ld FCFA\n",matricule,facture);
             printf("ENTRER (1) SI LE PAYEMENT A ETE EFFECTUÉ \n");
             printf("ENTRER (0) POUR SORTIR \n");    
             scanf("%d",&reponse);
             if (reponse == 1)
             {   
                 mise_a_jour_facture(matricule,facture);
                mise_a_jour_heure_sortie(matricule);
                id_place = recuperer_id(matricule);
                mise_a_jour_place(id_place ,0);
                
                

             }

             else{
                printf("bye\n");
                exit(1);
              }

               scanf("%c",&retour);
                    retourner();

             }

          break;

          case 3:
            printf("entrer le matricule de l'engin \n");
             scanf("%s",matricule); 
             facture = calcul_montant(matricule,"engin");
              if(facture !=0 ){
             printf("La facture de l'engin a deux roues immatriculé %s est : %ld FCFA\n",matricule,facture);
             printf("ENTRER (1) SI LE PAYEMENT A ETE EFFECTUÉ \n");
             printf("ENTRER (0) POUR SORTIR \n");    
             scanf("%d",&reponse);
             if (reponse == 1)
             {     
                 mise_a_jour_facture(matricule,facture);
                 mise_a_jour_heure_sortie(matricule);
                 /**je recuperere l'id de la place afin de mettre**/
                id_place = recuperer_id(matricule);
                if (id_place != 0)
                {
                    mise_a_jour_place(id_place ,0);
                }

             }

             else{
                printf("bye\n");
                exit(1);
             }
             }
              scanf("%c",&retour);
                    retourner();
          break;
           case 0:
         printf("bye\n");
         break;
         default:
         printf("choix indisponible veuillez entrer une reponse valide\n");
         menu();



    }

 }

 /** 
 *@brief Fonction sortie_parking permettant d'afficher les services d'affichage des places disponibles
*/

void gestion_parking(){

     int choix;
    printf("ENTRER (1) POUR AFFICHER LE NOMBRE DE PLACE DISPONIBLE\n");
    printf("ENTRER (2) POUR AFFICHER LES PLACES DISPONIBLES POUR LES CAMIONS \n");
    printf("ENTRER (3) POUR AFFICHER LES PLACES DISPONIBLES POUR LES VOITURES \n");
    printf("ENTRER (4) POUR AFFICHER LES PLACES DISPONIBLES POUR LES ENGINS A DEUX ROUES \n");
     printf("ENTRER (5) POUR AFFICHER LES VEHICULES STATIONNÉS \n");
    printf("ENTRER (0)  QUITTER\n");
    scanf("%d",&choix);
    switch(choix){
        case 1:
         place_disponible(); 
         break;
         case 2:
         place_disponible_pour_une_categorie("camion");
         break;
         case 3:
         place_disponible_pour_une_categorie("voiture");
         break;
         case 4:
         place_disponible_pour_une_categorie("engin");
         break;
         case 5:
         afficher_liste_des_stationnement();

         break;
         case 0:
         printf("bye\n");
         break;
         default:
         printf("choix indisponible veuillez entrer une reponse valide\n");
         menu();
    }
}



 



 /** 
 *@brief Fonction menu qui affiche les services disponibles
*/
void menu(){
    int choix;
    printf("\n\nENTRER (1) POUR GESTION DES ENTRÉES\n");
    printf("ENTRER (2) POUR GESTION DES SORTIES\n");
     printf("ENTRER (3) POUR GESTION DU PARKING\n");
      printf("ENTRER (4) AFFICHER L'HISTORIQUE\n");
    printf("ENTRER (0)  QUITTER\n");
    scanf("%d",&choix);
    switch(choix){
        case 1:
         entrer_parking();
         break;
         case 2:
         sortie_parking();
         break;
         case 3:
         gestion_parking();
         break;
         case 4:
         historique();
          scanf("%c",&retour);
                    retourner();
         break;
         case 0:
         printf("bye\n");
         break;
         default:
         printf("choix indisponible veuillez entrer une reponse valide\n");
         menu();
    }
}

