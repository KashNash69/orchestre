#ifndef CLIENT_ORCHESTRE_H
#define CLIENT_ORCHESTRE_H

// Ici toutes les communications entre l'orchestre et les clients :
// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
// - les deux tubes nommés pour la communication bidirectionnelle

#define NB_CLE 5 
#define FILE_CLE "CLIENT_ORCHESTRE/client_orchestre.h"

#define TUBE_O2C "pipe_o2c"
#define TUBE_C2O "pipe_c2o"

void creer_tube_sema_client_orchestre(int * pipe1, int * pipe2, int * sem);

void init_tube_client(int * pipe1, int * pipe2, int * sem);

void close_tube_sema_orchestre(int pipe1, int pipe2, int sem);

void close_tube_client(int pipe1, int pipe2);

#endif
