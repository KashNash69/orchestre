#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "myassert.h"
#include <sys/wait.h>
//tube
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//sema
#include <sys/ipc.h>
#include <sys/sem.h>

#include "config.h"
#include "../CLIENT_SERVICE/client_service.h"
#include "client_orchestre.h"
#include "orchestre_service.h"
#include "service.h"


static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <fichier config>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    if (argc != 2)
        usage(argv[0], "nombre paramètres incorrect");
    
    bool fin = false;

    // lecture du fichier de configuration
    config_init(argv[1]);

    // Pour la communication avec les clients
    // - création de 2 tubes nommés pour converser avec les clients
    // - création d'un sémaphore pour que deux clients ne
    //   ne communiquent pas en même temps avec l'orchestre
    int ret;
    
    int tube_o2c,tube_c2o,sem_client;
    
    creer_tube_sema_client_orchestre(&tube_o2c, &tube_c2o, &sem_client);
    
    // lancement des services, avec pour chaque service :
    // - création d'un tube anonyme pour converser (orchestre vers service)
    // - un sémaphore pour que le service préviene l'orchestre de la
    //   fin d'un traitement
    // - création de deux tubes nommés (pour chaque service) pour les
    //   communications entre les clients et les services

    int tube_s1,tube_s2,tube_s3;
    int sema_s1,sema_s2,sema_s3;
    creer_tube_sema_orchestre_service(&tube_s1,&tube_s2,&tube_s3, &sema_s1,&sema_s2,&sema_s3);
    
    int tube_s2c_1,tube_c2s_1;
    int tube_s2c_2,tube_c2s_2;
    int tube_s2c_3,tube_c2s_3;
    
    creer_tube_service(0,&tube_s2c_1,&tube_c2s_1);
    creer_tube_service(1,&tube_s2c_2,&tube_c2s_2);
    creer_tube_service(2,&tube_s2c_3,&tube_c2s_3);
    
    
    ret = fork();
    myassert(ret != -1, "fork service 1");
    //service 1
    if(ret == 0){
    
    	char * argv[6];
    	argv[0] = "service";
    	argv[1] = "1";
    	argv[2] = "0";
    	argv[3] = int_to_string(tube_s1);
    	argv[4] = int_to_string(tube_s2c_1);
    	argv[5] = int_to_string(tube_c2s_1);
    	
    	execv("service",argv);;
    	
    }
    ret = fork();
    myassert(ret != -1, "fork service 2");
    //service2
    if(ret == 0){
    char * argv[6];
    	argv[0] = "service";
    	argv[1] = "2";
    	argv[2] = "0";
    	argv[3] = int_to_string(tube_s2);
    	argv[4] = int_to_string(tube_s2c_2);
    	argv[5] = int_to_string(tube_c2s_2);
    	execv("service",argv);;
    	
    }
    
    ret = fork();
    myassert(ret != -1, "fork service 3");
    //service3
    if(ret == 0){
    char * argv[6];
    	argv[0] = "service";
    	argv[1] = "2";
    	argv[2] = "0";
    	argv[3] = int_to_string(tube_s3);
    	argv[4] = int_to_string(tube_s2c_3);
    	argv[5] = int_to_string(tube_c2s_3);
    	execv("service",argv);;
    	
    }
    
    fermer_tube_lecture(&tube_s1,&tube_s2,&tube_s3);

    while (! fin)
    {
        // ouverture ici des tubes nommés avec un client
        // attente d'une demande de service du client
        int numService;

        ouvrir_tube_orchestre(&tube_o2c, &tube_c2o);

        ret = read(tube_c2o,&numService, sizeof(int));
        myassert(ret == sizeof(int), "erreur lecture numService");
		
        // détecter la fin des traitements lancés précédemment via
        // les sémaphores dédiés (attention on n'attend pas la
        // fin des traitement, on note juste ceux qui sont finis)
        bool service1dispo, service2dispo,service3dispo;

        service1dispo = isServiceDispo(sema_s1);
        service2dispo = isServiceDispo(sema_s2);
        service3dispo = isServiceDispo(sema_s3);


        // analyse de la demande du client
        // si ordre de fin
        //     envoi au client d'un code d'acceptation (via le tube nommé)
        //     marquer le booléen de fin de la boucle
        // sinon si service non ouvert
        //     envoi au client d'un code d'erreur (via le tube nommé)
        // sinon si service déjà en cours de traitement
        //     envoi au client d'un code d'erreur (via le tube nommé)
        // sinon
        //     envoi au client d'un code d'acceptation (via le tube nommé)
        //     génération d'un mot de passe
        //     envoi d'un code de travail au service (via le tube anonyme)
        //     envoi du mot de passe au service (via le tube anonyme)
        //     envoi du mot de passe au client (via le tube nommé)
        //     envoi des noms des tubes nommés au client (via le tube nommé)
        // finsi

        if(numService == -1){
            ret = write(tube_o2c, "0" , sizeof(int));
            myassert(ret == sizeof(int), "envoie code fin d'orchestre");
            fin = !fin;
        }
        else if(numService == 0){
            if(service1dispo){
                ret = write(tube_o2c, "1" , sizeof(int));
                myassert(ret == sizeof(int), "envoie code acceptation");
                
                char * mdp = "SERVICE1";
                int lmdp = strlen(mdp);
                int lt1 = strlen(int_to_string(tube_c2s_1));
                int lt2 = strlen(int_to_string(tube_s2c_1));
                //     génération d'un mot de passe
                //envoie de la taille du mdp et le mdp au service
                envoyer(tube_s1, lmdp, mdp);
                //envoie de la taille du mdp et le mdp au client
                envoyer(tube_o2c, lmdp, mdp);
                //envoie taille et nom des tubes au client
                envoyer(tube_o2c, lt1, int_to_string(tube_c2s_1));
                envoyer(tube_o2c, lt2, int_to_string(tube_s2c_1));
            }
            else{
                ret = write(tube_o2c, "-1" , sizeof(int));
                myassert(ret == sizeof(int), "envoie code service indispo");
            }
        }
        else if (numService == 1){
            if(service2dispo){
                ret = write(tube_o2c, "1" , sizeof(int));
                myassert(ret == sizeof(int), "envoie code acceptation");
                
                char * mdp = "SERVICE2";
                int lmdp = strlen(mdp);
                int lt1 = strlen(int_to_string(tube_c2s_2));
                int lt2 = strlen(int_to_string(tube_s2c_2));
                //     génération d'un mot de passe
                //envoie de la taille du mdp et le mdp au service
                envoyer(tube_s2, lmdp, mdp);
                //envoie de la taille du mdp et le mdp au client
                envoyer(tube_o2c, lmdp, mdp);
                //envoie taille et nom des tubes au client
                envoyer(tube_o2c, lt1, int_to_string(tube_c2s_2));
                envoyer(tube_o2c, lt2, int_to_string(tube_s2c_2));      
            }
            else{
                ret = write(tube_o2c, "-1" , sizeof(int));
                myassert(ret == sizeof(int), "envoie code service indispo");
            }
        }
        else if(numService == 2){
            if(service3dispo){
                ret = write(tube_o2c,"1" , sizeof(int));
                myassert(ret == sizeof(int), "envoie code acceptation");
                
                char * mdp = "SERVICE3";
                int lmdp = strlen(mdp);
                int lt1 = strlen(int_to_string(tube_c2s_3));
                int lt2 = strlen(int_to_string(tube_s2c_3));
                //     génération d'un mot de passe
                //envoie de la taille du mdp et le mdp au service
                envoyer(tube_s3, lmdp, mdp);
                //envoie de la taille du mdp et le mdp au client
                envoyer(tube_o2c, lmdp, mdp);
                //envoie taille et nom des tubes au client
                envoyer(tube_o2c, lt1, int_to_string(tube_c2s_3));
                envoyer(tube_o2c, lt2, int_to_string(tube_s2c_3));

            }
            else{
                ret = write(tube_o2c, "-1" , sizeof(int));
                myassert(ret == sizeof(int), "envoie code service indispo");
            }
        }
        // attente d'un accusé de réception du client
        // fermer les tubes vers le client

        // il peut y avoir un problème si l'orchestre revient en haut de la
        // boucle avant que le client ait eu le temps de fermer les tubes
        // il faut attendre avec un sémaphore.
        // (en attendant on fait une attente d'1 seconde, à supprimer dès
        // que le sémaphore est en place)
        // attendre avec un sémaphore que le client ait fermé les tubes
        sleep(1);   // à supprimer
    }

    // attente de la fin des traitements en cours (via les sémaphores)

    // envoi à chaque service d'un code de fin

    // attente de la terminaison des processus services
    wait(NULL);
    wait(NULL);
    wait(NULL);

    // libération des ressources
    
    return EXIT_SUCCESS;
}
