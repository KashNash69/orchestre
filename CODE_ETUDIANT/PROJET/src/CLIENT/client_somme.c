#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//ajout de assert
#include <assert.h>
#include <unistd.h>

#include "client_service.h"
#include "client_somme.h"


/*----------------------------------------------*
 * usage pour le client somme
 *----------------------------------------------*/

static void usage(const char *exeName, const char *numService, const char *message)
{
    fprintf(stderr, "Client somme de deux nombres\n");
    fprintf(stderr, "usage : %s %s <n1> <n2> <prefixe>\n", exeName, numService);
    fprintf(stderr, "        %s         : numéro du service\n", numService);
    fprintf(stderr, "        <n1>      : premier nombre à sommer\n");
    fprintf(stderr, "        <n2>      : deuxième nombre à sommer\n");
    fprintf(stderr, "        <prefixe> : chaîne à afficher avant le résultat\n");
    fprintf(stderr, "exemple d'appel :\n");
    fprintf(stderr, "    %s %s 22 33 \"le résultat est : \"\n", exeName, numService);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

/*----------------------------------------------*
 * fonction de vérification des paramètres
 *----------------------------------------------*/

void client_somme_verifArgs(int argc, char * argv[])
{
    if (argc != 5)
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
// - les deux float dont on veut la somme
static void sendData(int fdW, int a , int b/* fd_pipe_to_service,*/ /* entier1, */ /* entier2 */)
{
    // envoi des deux nombres
    int ret ;
    
    ret = write(fdW, &a, sizeof(int));
    assert(ret == sizeof(int));
    ret = write(fdW, &b, sizeof(int));
    assert(ret == sizeof(int));
    
}

// ---------------------------------------------
// fonction de réception des résultats en provenance du service et affichage
// Les paramètres sont
// - le file descriptor du tube de communication en provenance du service
// - le prefixe
// - autre chose si nécessaire
static void receiveResult(int fdR, char * prefixe/* fd_pipe_from_service,*/ /* préfixe, */ /* autres paramètres si nécessaire */)
{
    // récupération de la somme
    int sum, ret;
    ret = read(fdR, &sum, sizeof(int));
    assert(ret == sizeof(int));

    
    // affichage du préfixe et du résultat
    
    printf("Le résultat est : %s %d\n", prefixe, sum);
}


// ---------------------------------------------
// Fonction appelée par le main pour gérer la communications avec le service
// Les paramètres sont
// - les deux file descriptors des tubes nommés avec le service
// - argc et argv fournis en ligne de commande
// Cette fonction analyse argv et en déduit les données à envoyer
//    - argv[2] : premier nombre
//    - argv[3] : deuxième nombre
//    - argv[4] : chaîne à afficher avant le résultat
void client_somme(int fdW, int fdR,/* fd des tubes avec le service, */ int argc, char * argv[])
{
    // variables locales éventuelles
    int a = (int) *argv[2];
    int b = (int) *argv[3];
        
    sendData(fdW, a, b);
    receiveResult(fdR, argv[4]);
}

