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


int creer_tube_sema_client_orchestre(){
    int pipe_o2c;
    int pipe_c2o;
    int ret;

    ret = pipe(pipe_o2c);
    myassert((ret == 0), "Erreur pipe tube orchestre to client" ) ;

    ret = pipe(pipe_c2o);
    myassert((ret == 0), "Erreur pipe tube client to orchestre" ) ;

    key_t cle = ftok(FILE_CLE,NB_CLE);
    myassert((cle != -1), ("Cle avec l'orchestre erreur")); 

    int sem = semget(cle, 1, IPC_CREAT | IPC_EXCL | 0641);

}
