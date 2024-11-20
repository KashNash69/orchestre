#ifndef CLIENT_SERVICE_H
#define CLIENT_SERVICE_H

// Ici toutes les communications entre les services et les clients :
// - les deux tubes nommés pour la communication bidirectionnelle


int init_client_service();

//service 1 : somme
int pipe_s2c_1[2];
int pipe_c2s_1[2];

//service 2 : compression
int pipe_s2c_2[2];
int pipe_c2s_2[2];

//service 3 : sigma
int pipe_s2c_3[2];
int pipe_c2s_3[2];



#endif
