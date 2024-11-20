#include "myassert.h"

#include "client_service.h"
#include "myassert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int init_client_service(){

    //service 1 : somme
    int pipe_s2c_1;
    int pipe_c2s_1;

    //service 2 : compression
    int pipe_s2c_2[2];
    int pipe_c2s_2[2];

    //service 3 : sigma
    int pipe_s2c_3[2];
    int pipe_c2s_3[2];

    int ret;

    ret = pipe(pipe_s2c_1);
    myassert((ret == 0), "Erreur pipe tube service to client : somme" ) ;

    ret = pipe(pipe_c2s_1);
    myassert((ret == 0), "Erreur pipe tube client to service : somme" ) ;

    ret = pipe(pipe_s2c_2);
    myassert((ret == 0), "Erreur pipe tube service to client : compression" ) ;

    ret = pipe(pipe_c2s_2);
    myassert((ret == 0), "Erreur pipe tube client to service : compression" ) ;

    ret = pipe(pipe_s2c_3);
    myassert((ret == 0), "Erreur pipe tube service to client : sigma" ) ;

    ret = pipe(pipe_c2s_3);
    myassert((ret == 0), "Erreur pipe tube client to service : sigma" ) ;
}
