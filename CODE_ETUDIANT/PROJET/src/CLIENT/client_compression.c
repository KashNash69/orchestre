#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//ajout de assert
#include <assert.h>
#include <unistd.h>

#include "client_service.h"
#include "client_compression.h"


/*----------------------------------------------*
 * usage pour le client compression
 *----------------------------------------------*/

static void usage(const char *exeName, const char *numService, const char *message)
{
    fprintf(stderr, "Client compression de chaîne\n");
    fprintf(stderr, "usage : %s %s <chaîne>\n", exeName, numService);
    fprintf(stderr, "        %s      : numéro du service\n", numService);
    fprintf(stderr, "        <chaine> : chaîne à compresser\n");
    fprintf(stderr, "exemple d'appel :\n");
    fprintf(stderr, "    %s %s \"aaabbcdddd\"\n", exeName, numService);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/*----------------------------------------------*
 * fonction de vérification des paramètres
 *----------------------------------------------*/

void client_compression_verifArgs(int argc, char * argv[])
{
    if (argc != 3)
        usage(argv[0], argv[1], "nombre d'arguments");
    // éventuellement d'autres tests
}


/*----------------------------------------------*
 * fonctions de communication avec le service
 *----------------------------------------------*/

// ---------------------------------------------
// fonction d'envoi des données du client au service
// Les paramètres sont
// - le file descriptor du tube de communication vers le service
// - la chaîne devant être compressée
static void sendData(int fdW, char * chaine)
{
    // envoi de la chaîne à compresser

    int ret;

    ret = write(fdW, &chaine, sizeof(char *));
    assert(ret == sizeof(char *));
}

// ---------------------------------------------
// fonction de réception des résultats en provenance du service et affichage
// Les paramètres sont
// - le file descriptor du tube de communication en provenance du service
// - autre chose si nécessaire
static void receiveResult(int fdR/* fd_pipe_from_service,*/ /* autres paramètres si nécessaire */)
{
    // récupération de la chaîne compressée
    int ret;
    char * result;
    ret = read(fdR, &result, sizeof(char*));
    assert(ret  == sizeof(char* ));
    // affichage du résultat
    
    printf("Le résultat est : %s\n", result);
}

// ---------------------------------------------
// Fonction appelée par le main pour gérer la communications avec le service
// Les paramètres sont
// - les deux file descriptors des tubes nommés avec le service
// - argc et argv fournis en ligne de commande
// Cette fonction analyse argv et en déduit les données à envoyer
//    - argv[2] : la chaîne à compresser
void client_compression(int fdW, int fdR,/* fd des tubes avec le service, */ int argc, char * argv[])
{
    // variables locales éventuelles
    sendData(fdW, argv[2]);
    receiveResult(fdR);
}

