#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp4.h"




int main()
{
    //On commence par cree un index vide
    T_Index* index = NULL;

    index = malloc(sizeof(T_Index));
    if(index != NULL){
        index->nbMotsDistincts = 0;
        index->nbMotsTotal= 0;
        index->racine = NULL;
    }



    // ============= MENU UTILISATEUR ============= */
    char choix = '0';
    while (choix != '9') {
        printf("\n======================================");
        printf("\n======================================");
        printf("\n1. Charger un fichier");
        printf("\n2. Caractéristiques de l'index");
        printf("\n3. Caractéristiques de l'index");
        printf("\n4. Rechercher un mot");
        printf("\n5. Afficher les occurrences d’un mot");
        printf("\n6. Construire le texte à partir de l’index");
        printf("\n7. Quitter");
        printf("\n======================================");
        printf("\n   Votre choix ? ");
        choix = getchar();
        viderBuffer(); // supprimer de l'entree standard le retour charriot et les eventuels caracteres supplementaires tapes par l'utilisateur

        switch (choix) {
            case '1' : ;
                //A chaque fois qu'on charge un nouveau fichier, on ecrase l'index courant pour en creer un nouveau
                index->nbMotsDistincts = 0;
                index->nbMotsTotal= 0;
                index->racine = NULL;


                char nom_fichier[100];
                printf("Entrez le nom du fichier que vous voulez charger (Il doit être dans le même dossier que ce programme");
                scanf("%c", &nom_fichier);

                break;

            case '2' : ;

                break;

             case '3' :

                break;

            case '4' :

                break;

            case '5' : ;

                break;

            case '6' :

                break;

            case '7' :
                printf("\n======== PROGRAMME TERMINE ========\n");
                viderBuffer();
                break;

            default :
                printf("\n\nERREUR : votre choix n'est pas valide ! ");
        }
        printf("\n\n\n");
    }
    return 0;
}
