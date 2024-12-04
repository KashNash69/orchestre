#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "io.h"
//tube
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//sema
#include <sys/ipc.h>
#include <sys/sem.h>

#include "orchestre_service.h"
#include "client_service.h"

#include "service_compression.h"

// définition éventuelle de types pour stocker les données


/*----------------------------------------------*
 * fonctions appelables par le service
 *----------------------------------------------*/

// fonction de réception des données
static void receiveData(/* fd_pipe_from_client, */ /* données à récupérer */)
{
}

// fonction de traitement des données
static void computeResult(char * chaine/* données récupérées, */ /* résultat */)
{
	int len = strlen(chaine);
	int count = 1;
	int size = 3;
	char * sortie = (char * ) malloc(size*sizeof(char));
	char tampon[2];
	char * inttochar;
	tampon[0] = chaine[0];
	tampon[1] = '\0';
	sortie[0] = '\0';
	for(int i = 1 ; i < len + 1 ; i++) {
		if (i < len) {
			if (chaine[i] == tampon[0]) {
				count++;
				}
			else {
				inttochar = io_intToStr(count);
				size = size + 2 + strlen(inttochar);
				sortie = realloc(sortie,  size * sizeof(char));
				sortie = strcat(sortie, strcat(inttochar, tampon));
				count = 1;
				tampon[0] = chaine[i];
			}
		}
		else {
			inttochar = io_intToStr(count);
			size = size + 2 + strlen(inttochar);
			sortie = realloc(sortie,  size * sizeof(char));
			sortie = strcat(sortie, strcat(inttochar, tampon));
	}	
	return sortie; 	
}

// fonction d'envoi du résultat
static void sendResult(/* fd_pipe_to_client,*/ /* résultat */)
{
}


/*----------------------------------------------*
 * fonction appelable par le main
 *----------------------------------------------*/
void service_compression(/* fd tubes nommés avec un client */)
{
    // initialisations diverses
    
    receiveData(/* paramètres */);
    computeResult(/* paramètres */);
    sendResult(/* paramètres */);

    // libération éventuelle de ressources
}
