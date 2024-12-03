#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "orchestre_service.h"
#include "client_service.h"
#include "service.h"
#include "service_somme.h"
#include "service_compression.h"
#include "service_sigma.h"


static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <num_service> <clé_sémaphore> <fd_tube_anonyme> "
            "<nom_tube_service_vers_client> <nom_tube_client_vers_service>\n",
            exeName);
    fprintf(stderr, "        <num_service>     : entre 0 et %d\n", SERVICE_NB - 1);
    fprintf(stderr, "        <clé_sémaphore>   : entre ce service et l'orchestre (clé au sens ftok)\n");
    fprintf(stderr, "        <fd_tube_anonyme> : entre ce service et l'orchestre\n");
    fprintf(stderr, "        <nom_tube_...>    : noms des deux tubes nommés reliés à ce service\n");
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    //initialisations diverses : analyse de argv
    if (argc != 6) {
        usage(argv[0], "nombre paramètres incorrect");
    }
        int num_service = atoi(argv[1]);
        int cleSemaphore = atoi(argv[2]);
        int fd_tube_anonyme = atoi(argv[3]);
        int tube_s2c = atoi(argv[4]);
        int tube_c2s = atoi(argv[5]);
    
        // attente d'un code de l'orchestre (via tube anonyme)
        // si code de fin
        //    sortie de la boucle
        // sinon
        //    réception du mot de passe de l'orchestre
        //    ouverture des deux tubes nommés avec le client
        //    attente du mot de passe du client
        //    si mot de passe incorrect
        //        envoi au client d'un code d'erreur
        //    sinon
        //        envoi au client d'un code d'acceptation
        //        appel de la fonction de communication avec le client :
        //            une fct par service selon numService (cf. argv[1]) :
        //                   . service_somme
        //                ou . service_compression
        //                ou . service_sigma
        //        attente de l'accusé de réception du client
        //    finsi
        //    fermeture ici des deux tubes nommés avec le client
        //    modification du sémaphore pour prévenir l'orchestre de la fin
        // finsi

    while (true) {
        // Lire un mot de passe depuis le tube anonyme (orchestre -> service)
        char password_orchestre[128];
        size_t read_ret = read(fd_tube_anonyme, password_orchestre, sizeof(password_orchestre));
        myassert(read_ret > 0, "Erreur de lecture du tube anonyme");
        password_orchestre[read_ret] = '\0'; // Null terminate

        // Vérifier si on a reçu le signal d'arrêt
        if (strcmp(password_orchestre, "END") == 0) {
            break;
        }

        // Ouvrir les tubes nommés entre le client et le service
        ouvrir_tube_service(num_service, &tube_s2c, &tube_c2s);

        // Lire le mot de passe du client
        char password_client[128];
        read_ret = read(tube_c2s, password_client, sizeof(password_client));
        if (read_ret <= 0) {
            perror("Erreur de lecture du mot de passe client");
            close_tube_client(pipe_from_client, pipe_to_client);
            continue;
        }
        password_client[read_ret] = '\0';

        // Valider le mot de passe
        if (strcmp(password_orchestre, password_client) != 0) {
            // Mot de passe incorrect
            write(pipe_to_client, "ERROR", strlen("ERROR"));
        } else {
            // Mot de passe correct, exécuter le service
            switch (num_service) {
            case SERVICE_SOMME:
                service_somme(pipe_from_client, pipe_to_client);
                break;
            case SERVICE_COMPRESSION:
                service_compression(pipe_from_client, pipe_to_client);
                break;
            case SERVICE_SIGMA:
                service_sigma(pipe_from_client, pipe_to_client);
                break;
            default:
                fprintf(stderr, "Service inconnu\n");
                break;
            }

            // Envoyer un code de succès au client
            write(pipe_to_client, "OK", strlen("OK"));
        }

        // Fermer les tubes pour le client
        close_tube_client(pipe_from_client, pipe_to_client);

        // Notifier l'orchestre que le traitement est terminé
        struct sembuf sem_op = {0, 1, 0}; // Incrémenter le sémaphore
        myassert(semop(cle_semaphore, &sem_op, 1) == -1, "Erreur lors de la notification à l'orchestre")
    }

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}
