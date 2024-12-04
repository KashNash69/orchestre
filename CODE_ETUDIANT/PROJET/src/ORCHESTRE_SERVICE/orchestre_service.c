#include "orchestre_service.h"
#include "myassert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
//tube
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//sema
#include <sys/ipc.h>
#include <sys/sem.h>

void fermer_tube_lecture(int * pipe1, int * pipe2, int* pipe3){
    int ret = close(pipe1[0]);//on ferme les extremitées inutiles // peut etre il faut mettre *pipe1 ??
    myassert(ret == 0, "fermeture tube 1 ano coté lecture");
    ret = close(pipe2[0]);//on ferme les extremitées inutiles
    myassert(ret == 0, "fermeture tube 2 ano coté lecture");
    ret = close(pipe3[0]);//on ferme les extremitées inutiles
    myassert(ret == 0, "fermeture tube 3 ano coté lecture");
}

void fermer_tube_ecriture(int * pipe1, int * pipe2, int* pipe3){
    int ret = close(pipe1[1]);//on ferme les extremitées inutiles // peut etre il faut mettre *pipe1 ??
    myassert(ret == 0, "fermeture tube 1 ano coté ecriture");
    ret = close(pipe2[1]);//on ferme les extremitées inutiles
    myassert(ret == 0, "fermeture tube 2 ano coté ecriture");
    ret = close(pipe3[1]);//on ferme les extremitées inutiles
    myassert(ret == 0, "fermeture tube 3 ano coté ecriture");
}

void creer_tube_sema_orchestre_service(int * pipe1,int * pipe2,int * pipe3,int * sem1,int * sem2,int * sem3){
	int pipe_s1[2];
	int pipe_s2[2];
	int pipe_s3[2];
	
	int ret = pipe(pipe_s1);
    myassert(ret == 0, "erreur creation tube orchestre service 1");
    ret = pipe(pipe_s2);
    myassert(ret == 0, "erreur creation tube orchestre service 2");
    ret = pipe(pipe_s3);
    myassert(ret == 0, "erreur creation tube orchestre service 3");
    
    
	key_t cle1 = ftok(FILE_CLE_S,NB_CLE1);
    myassert((cle1 != -1), ("Cle avec service 1 erreur")); 

	key_t cle2 = ftok(FILE_CLE_S,NB_CLE2);
    myassert((cle2 != -1), ("Cle avec service 2 erreur")); 
    
	key_t cle3 = ftok(FILE_CLE_S,NB_CLE3);
    myassert((cle3 != -1), ("Cle avec service 3 erreur")); 
    
    
    int sema1 = semget(cle1, 1, IPC_CREAT | IPC_EXCL | 0641);
    myassert(sema1 != -1, "erreur creation sémaphore service 1");
    
    int sema2 = semget(cle2, 1, IPC_CREAT | IPC_EXCL | 0641);
    myassert(sema2 != -1, "erreur creation sémaphore service 2");
    
    int sema3 = semget(cle3, 1, IPC_CREAT | IPC_EXCL | 0641);
    myassert(sema3 != -1, "erreur creation sémaphore service 3");
    
	fermer_tube_lecture(pipe_s1, pipe_s2, pipe_s3);
    
    pipe1 = pipe_s1;
    pipe2 = pipe_s2;
    pipe3 = pipe_s3;

    *sem1 = sema1;
    *sem2 = sema2;
    *sem3 = sema3;
}

_Bool isServiceDispo(int sem){  
    return (sem == 1);
}

void envoyer(int tube, int taille, char * message){
    int ret;

    ret = write(tube, &taille , sizeof(int));
    myassert(ret ==  sizeof(int), ("envoie taille %s", message));

    ret = write(tube, &message , taille*sizeof(int));
    myassert(ret == taille * sizeof(int), ("envoie %s",message));

}


//pour l'ochestre il faut fermer chaque tube en lecture ( close(tube[0]))
//pour chaque service( fils de l'ochestre + exec) il faut fermer en écriture close((tube[1]))

	




