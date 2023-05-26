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
    while (choix != '7') {
        printf("\n======================================");
        printf("\n======================================");
        printf("\n1. Charger un fichier");
        printf("\n2. Caract�ristiques de l'index");
        printf("\n3. Caract�ristiques de l'index");
        printf("\n4. Rechercher un mot");
        printf("\n5. Afficher les occurrences d�un mot");
        printf("\n6. Construire le texte � partir de l�index");
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
                printf("Entrez le nom du fichier que vous voulez charger (Il doit �tre dans le m�me dossier que ce programme)\n");
                scanf("%c", &nom_fichier);
                indexerFichier(index, nom_fichier);
                viderBuffer();

                break;

            case '2' : ;
                printf("=====Caracteristiques de l'index======\n");
                printf("Nombres de mots distincts : %d\n", index->nbMotsDistincts);
                printf("Nombres de mots total : %d\n", index->nbMotsTotal);
                viderBuffer();
                break;

             case '3' :

                break;

            case '4' :

            char motRecherche[100];
            printf("Entrez le mot que vous voulez rechercher : ");
            scanf("%c", &motRecherche);

            // Appel de la fonction de recherche
            T_Noeud* resultat = rechercherMot(index, motRecherche);

            if (resultat != NULL) {
                printf("%s : \n", resultat->mot);
                T_Position* parcour = resultat->listePositions;
                for(int i=0;i<resultat->nbOccurences;i++){
                    printf("")
                    printf("Numero de ligne : %d", parcour->numeroLigne);
                    printf("Position dans la ligne ; %d", parcour->ordre);
                    printf("Numero de phrase : %d", parcour->numeroPhrase);

                    parcour = parcour->suivant;
                }
            } else {
                printf("Mot non trouve.\n");
            }


                break;

            case '5' : ;

                break;

            case '6' :

                break;

            case '7' :
                printf("\n======== PROGRAMME TERMINE ========\n");
                //Fiare une boucle qui free les noeuds et les positions
                free(index);
                viderBuffer();
                break;

            default :
                printf("\n\nERREUR : votre choix n'est pas valide ! ");
        }
        printf("\n\n\n");
    }
    return 0;
}
