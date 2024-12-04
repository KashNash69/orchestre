#include "client_orchestre.h"
#include "myassert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//tube
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//sema
#include <sys/ipc.h>
#include <sys/sem.h>


void creer_tube_sema_client_orchestre(int * sem){
    int pipe_o2c = mkfifo(TUBE_O2C, 0644);
    myassert(pipe_o2c != -1, "erreur creation tube orchestre client 1");
    int pipe_c2o = mkfifo(TUBE_C2O, 0644);
    myassert(pipe_c2o == 0, "erreur creation tube orchestre client 2");
 
    key_t cle = ftok(FILE_CLE,NB_CLE);
    myassert((cle != -1), ("Cle avec l'orchestre erreur")); 

    int sema = semget(cle, 1, IPC_CREAT | IPC_EXCL | 0641);
    
    myassert(sema != -1, "erreur creation sémaphore orchestre");
    sema = semctl(sema,0,SETVAL);

    *sem = sema;
}

void ouvrir_tube_orchestre(int * pipe1, int * pipe2){
    //OUVERTURE 
    int pipe_o2c = open(TUBE_O2C, O_WRONLY);//orchestre en ecriture
    myassert(pipe_o2c == 0, "erreur ouverture tube orchestre client 1");
    int pipe_c2o = open(TUBE_C2O, O_RDONLY);//client en lecture
    myassert(pipe_o2c == 0, "erreur ouverture tube orchestre client 2");

    *pipe1 = pipe_o2c;
    *pipe2 = pipe_c2o;

}

void init_tube_client(int * pipe1, int * pipe2){
    //OUVERTURE
    int pipe_c2o = open(TUBE_C2O, O_WRONLY);//client en ecriture
    myassert(pipe_c2o == 0, "erreur ouverture tube client orchestre 1");
    int pipe_o2c = open(TUBE_O2C, O_RDONLY);//orchestre en lecture
    myassert(pipe_o2c == 0, "erreur ouverture tube client orchestre 2");

    *pipe1 = pipe_o2c; 
    *pipe2 = pipe_c2o;
}

void init_sem_client(int * sem){

    key_t cle = ftok(FILE_CLE,NB_CLE);
    myassert((cle != -1), ("Cle avec l'orchestre erreur"));

    int sema = semget(cle, 1, 0);
    myassert(sema != -1, "erreur recupération sémaphore client");
 
    *sem = sema;
}

void close_tube_orchestre(int pipe1, int pipe2){
    int ret = close(pipe1);
    myassert((ret ==0 ), ("fermeture tube orchestre client lecture\n"));
    ret = close(pipe2);
    myassert((ret ==0 ), ("fermeture tube orchestre client ecriture\n"));
}

void unlink_tube_orchestre(){
    int ret = unlink(TUBE_C2O);
    myassert((ret ==0 ), ("destruction tube orchestre client lecture\n"));
    ret = unlink(TUBE_O2C);
    myassert((ret ==0 ), ("destruction tube orchestre client ecriture\n"));

}

void kill_semaphore_orchestre(int sem){
    int ret = semctl(sem, -1, IPC_RMID);
    myassert((ret != -1), ("fermeture semaphore orchestre client\n"));
}

void close_tube_client(int pipe1, int pipe2){
    int ret = close(pipe1);
    myassert((ret ==0 ), ("fermeture tube client orchestre ecriture\n"));
    ret = close(pipe2);
    myassert((ret ==0 ), ("fermeture tube client orchestre lecture\n"));  
}

void entrerSC(int * sem){
    struct sembuf opMoins = {0, -1, 0};
    int ret = semop(*sem, &opMoins, 1); 
    myassert(ret != -1, "erreur entrer SC");
}

void sortirSC(int * sem){  
    struct sembuf opPlus = {0, +1, 0};
    int ret = semop(*sem, &opPlus, 1); 
    myassert(ret != -1, "erreur sortir SC");
}