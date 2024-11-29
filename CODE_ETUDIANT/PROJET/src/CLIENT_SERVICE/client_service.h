#ifndef CLIENT_SERVICE_H
#define CLIENT_SERVICE_H

// Ici toutes les communications entre les services et les clients :
// - les deux tubes nommés pour la communication bidirectionnelle

#define TUBE_S2C_1 "pipe_s2c_1"
#define TUBE_C2S_1 "pipe_c2s_1"

#define TUBE_S2C_2 "pipe_s2c_2"
#define TUBE_C2S_2 "pipe_c2s_2"

#define TUBE_S2C_3 "pipe_s2c_3"
#define TUBE_C2S_3 "pipe_c2s_3"

void ouvrir_tube_service(int nb_service, int *pipe1, int * pipe2);

void creer_tube_service_1(int *pipe1, int * pipe2);
void creer_tube_service_2(int *pipe1, int * pipe2);
void creer_tube_service_3(int *pipe1, int * pipe2);

void ouvrir_tube_client(int nb_service, int *pipe1, int * pipe2);

void close_tube_service(int nb_service,int pipe1, int pipe2);

void close_tube_client(int pipe1, int pipe2);

#endif
