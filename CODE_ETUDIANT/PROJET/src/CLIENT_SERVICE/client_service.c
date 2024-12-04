#include "client_service.h"
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

void ouvrir_tube_service(int nb_service, int *pipe1, int * pipe2){
	myassert(nb_service < 3 && nb_service > -1, "erreur ouvrir tube mauvais nb service");

	int pipe_s2c, pipe_c2s;
		
    //OUVERTURE 
    switch(nb_service){
    	case 0 :
			pipe_s2c = open(TUBE_S2C_1, O_WRONLY);//service en ecriture
			myassert(pipe_s2c != -1, "erreur ouverture tube service 1 client 1");
			pipe_c2s = open(TUBE_C2S_1, O_RDONLY);//client en lecture
			myassert(pipe_c2s != -1, "erreur ouverture tube service 1 client 2");
			break;
    
    	case 1 : 
			pipe_s2c = open(TUBE_S2C_2, O_WRONLY);//service en ecriture
			myassert(pipe_s2c != -1, "erreur ouverture tube service 2 client 1");
			pipe_c2s = open(TUBE_C2S_2, O_RDONLY);//client en lecture
			myassert(pipe_c2s != -1, "erreur ouverture tube service 2 client 2");
			break;

    	case 2 : 
			pipe_s2c = open(TUBE_S2C_3, O_WRONLY);//service en ecriture
			myassert(pipe_s2c != -1, "erreur ouverture tube service 3 client 1");
			pipe_c2s = open(TUBE_C2S_3, O_RDONLY);//client en lecture
			myassert(pipe_c2s != -1, "erreur ouverture tube service 3 client 2");
			break;
    }	
   	*pipe1 = pipe_s2c;
	*pipe2 = pipe_c2s;
}

void creer_tube_service(int nb_service){
	myassert(nb_service < 3 && nb_service > -1, "erreur ouvrir tube mauvais nb service");

	int pipe_s2c, pipe_c2s;
	
	switch(nb_service){
		case 0:
			pipe_s2c = mkfifo(TUBE_S2C_1, 0644);
			myassert(pipe_s2c == 0, "erreur creation tube service 1 client 1");
			pipe_c2s = mkfifo(TUBE_C2S_1, 0644);
			myassert(pipe_c2s == 0, "erreur creation tube service 1 client 2");
			break;
		case 1:
			pipe_s2c = mkfifo(TUBE_S2C_2, 0644);
			myassert(pipe_s2c == 0, "erreur creation tube service 2 client 1");
			pipe_c2s = mkfifo(TUBE_C2S_2, 0644);
			myassert(pipe_c2s == 0, "erreur creation tube service 2 client 2");
			break;
		case 2:
			pipe_s2c = mkfifo(TUBE_S2C_3, 0644);
			myassert(pipe_s2c == 0, "erreur creation tube service 2 client 1");
			pipe_c2s = mkfifo(TUBE_C2S_3, 0644);
			myassert(pipe_c2s == 0, "erreur creation tube service 2 client 2");
			break;
	}
 	
}


void ouvrir_tube_client(int nb_service, int *pipe1, int * pipe2){
	myassert(nb_service < 3 && nb_service > -1, "erreur ouvrir tube mauvais nb service");

	int pipe_s2c, pipe_c2s;

	switch(nb_service){
		case 0 : 
			pipe_s2c = open(TUBE_S2C_1, O_RDONLY);//service en lecture
			myassert(pipe_s2c != -1, "erreur ouverture tube client service 1 1");
			pipe_c2s = open(TUBE_C2S_1, O_WRONLY);//client en ecriture
			myassert(pipe_c2s != -1, "erreur ouverture tube  client service 1 2");
			break;
		case 1 :
			pipe_s2c = open(TUBE_S2C_2, O_RDONLY);//service en lecture
			myassert(pipe_s2c != -1, "erreur ouverture tube client service 2 1");
			pipe_c2s = open(TUBE_C2S_2, O_WRONLY);//client en ecriture
			myassert(pipe_c2s != -1, "erreur ouverture tube client service 2 2");
			break;
		case 2 :
			pipe_s2c = open(TUBE_S2C_3, O_RDONLY);//service en lecture
			myassert(pipe_s2c != -1, "erreur ouverture tube client service 3 1");
			pipe_c2s = open(TUBE_C2S_3, O_WRONLY);//client en ecriture
			myassert(pipe_c2s != -1, "erreur ouverture tube client service 3 2");
			break;
	}
	*pipe2 = pipe_s2c;
	*pipe1 = pipe_c2s;
}

void close_tube_service(int nb_service,int pipe1, int pipe2){
	myassert(nb_service < 3 && nb_service > -1, "erreur ouvrir tube mauvais nb service");

	int ret;
	switch(nb_service){
		case 0 :
			ret = close(pipe1);
			myassert((ret ==0 ), ("fermeture tube service 1 client lecture\n"));
			ret = close(pipe2);
			myassert((ret ==0 ), ("fermeture tube service 1 client ecriture\n"));

			ret = unlink(TUBE_S2C_1);
			myassert((ret ==0 ), ("destruction tube service 1 client lecture\n"));
			ret = unlink(TUBE_C2S_1);
			myassert((ret ==0 ), ("destruction tube service 1 client ecriture\n"));
			break;
		case 1 :
			ret = close(pipe1);
			myassert((ret ==0 ), ("fermeture tube service 2 client lecture\n"));
			ret = close(pipe2);
			myassert((ret ==0 ), ("fermeture tube service 2 client ecriture\n"));

			ret = unlink(TUBE_S2C_2);
			myassert((ret ==0 ), ("destruction tube service 2 client lecture\n"));
			ret = unlink(TUBE_C2S_2);
			myassert((ret ==0 ), ("destruction tube service 2 client ecriture\n"));
			break;
		case 2 :
			ret = close(pipe1);
			myassert((ret ==0 ), ("fermeture tube service 3 client lecture\n"));
			ret = close(pipe2);
			myassert((ret ==0 ), ("fermeture tube service 3 client ecriture\n"));

			ret = unlink(TUBE_S2C_3);
			myassert((ret ==0 ), ("destruction tube service 3 client lecture\n"));
			ret = unlink(TUBE_C2S_3);
			myassert((ret ==0 ), ("destruction tube service 3 client ecriture\n"));
			break;	
	}
}

void close_tube_client_service(int pipe1, int pipe2){
    int ret = close(pipe1);
    myassert((ret ==0 ), ("fermeture tube client ecriture\n"));
    ret = close(pipe2);
    myassert((ret ==0 ), ("fermeture tube client lecture\n"));  
}

