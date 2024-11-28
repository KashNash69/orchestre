#ifndef ORCHESTRE_SERVICE_H
#define ORCHESTRE_SERVICE_H

// Ici toutes les communications entre l'orchestre et les services :
// - le tube anonyme pour que l'orchestre envoie des données au service
// - le sémaphore pour que  le service indique à l'orchestre la fin
//   d'un traitement

#define NB_CLE1 1
#define NB_CLE2 2
#define NB_CLE3 3
#define FILE_CLE "ORCHESTRE_SERVICE/orchestre_service.h"

void fermer_tube_lecture(int * pipe1, int * pipe2, int* pipe3);

void fermer_tube_ecriture(int * pipe1, int * pipe2, int* pipe3);

void creer_tube_sema_orchestre_service(int * pipe1,int * pipe2,int * pipe3,int * sem1,int * sem2,int * sem3);


#endif
