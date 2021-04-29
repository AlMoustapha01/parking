execution : compilation
	 ./gerer_parking
compilation :gerer_parking.c
	gcc affichage.c connexion.c facturation.c parking.c stationnement.c gerer_parking.c -o gerer_parking -lmysqlclient

