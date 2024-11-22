#include "myassert.h"

#include "client_orchestre.h"
#include "myassert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>


int creer_tube_sema_client_orchestre(){
    int pipe_o2c = mkfifo(TUBE_O2C);
    int pipe_c2o = mkfifo(TUBE_C2O);//creer des tubes nommés 
    //ne pas les ouvrir c'est l'orchestre qui le fera

    //OUVERTURE 
    int pipe_o2c = open(TUBE_O2C, O_WRONLY);
    int pipe_c2o = open(TUBE_C2O, O_RDONLY);

    key_t cle = ftok(FILE_CLE,NB_CLE);
    myassert((cle != -1), ("Cle avec l'orchestre erreur")); 

    int sem = semget(cle, 1, IPC_CREAT | IPC_EXCL | 0641);

}

int init_tube_client(){
    //OUVERTURE
    int pipe_c2o = open(TUBE_C2O, O_WRONLY);
    int pipe_o2c = open(TUBE_O2C, O_RDONLY);
}

int close_tube_client(){
    int ret = close(TUBE_C2O);
    myassert((ret ==0 ), ("fermeture tube client orchestre\n"));
    ret = close(TUBE_O2C);
    myassert((ret ==0 ), ("fermeture tube client orchestre\n"));  
}

int close_tube_sema_orchestre(){
    int ret = close(TUBE_C2O);
    myassert((ret ==0 ), ("fermeture tube orchestre client\n"));
    ret = close(TUBE_O2C);
    myassert((ret ==0 ), ("fermeture tube orchestre client\n"));

    //fermer le sémaphore
}
