#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "orchestre_service.h"
#include "client_service.h"

#include "service_somme.h"

// définition éventuelle de types pour stocker les données


/*----------------------------------------------*
 * fonctions appelables par le service
 *----------------------------------------------*/

// fonction de réception des données
static void receiveData(/* fd_pipe_from_client, */ /* données à récupérer */)
{
}

// fonction de traitement des données
static void computeResult(float f1, float f2, float * f3){
	*f3 = f1 + f2;
}

// fonction d'envoi du résultat
static void sendResult(/* fd_pipe_to_client,*/ /* résultat */)
{
}


/*----------------------------------------------*
 * fonction appelable par le main
 *----------------------------------------------*/
void service_somme(/* fd tubes nommés avec un client */)
{
    // initialisations diverses
    
    receiveData(/* paramètres */);
    computeResult(/* paramètres */);
    sendResult(/* paramètres */);

    // libération éventuelle de ressources
}
