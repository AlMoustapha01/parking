


MYSQL mysql;
 void retourner();
int connexion();
int verification(char *type);
int enregister_client(char *type ,char *mat,char *nom ,char *tel);
long duree_heure(char *matricule);
long calcul_montant(char *matricule ,char *type);
int recuperer_id(char *matricule);
void mise_a_jour_place (int id , int statut);
 void entrer_parking();
  void sortie_parking();
void place_disponible();
void place_disponible_pour_une_categorie(char *type);
void ajouter_parking();
void supprimer_client( char *matricule);
int client_existe(char *matricule);
void mise_a_jour_heure_sortie(char *matricule);
void afficher_liste_des_stationnement();
void mise_a_jour_facture(char *matricule,long facture);
void historique();
  void menu();
