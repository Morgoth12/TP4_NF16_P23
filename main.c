#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp4v2.h"




int main()
{
    //On commence par cree un index vide
    T_Index* index = NULL;

    index = malloc(sizeof(T_Index));
    if(index != NULL){
        index->nbMotsDistincts = 0;
        index->nbMotsTotal= 0;
        index->racine = NULL;
        index->liste_phrase = NULL;
    }



    // ============= MENU UTILISATEUR ============= */
    char choix = '0';
    while (choix != '7') {
        printf("\n======================================");
        printf("\n======================================");
        printf("\n1. Charger un fichier");
        printf("\n2. Caracteristiques de l index");
        printf("\n3. Afficher index");
        printf("\n4. Rechercher un mot");
        printf("\n5. Afficher les occurrences d un mot");
        printf("\n6. Construire le texte a partir de l index");
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
                printf("Entrez le nom du fichier que vous voulez charger (Il doit etre dans le meme dossier que ce programme)\n");
                scanf("%s", &nom_fichier);
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
                afficherIndex(index);
                break;

            case '4' :;

                char motRecherche[100];
                printf("Entrez le mot que vous voulez rechercher : ");
                scanf(" %s", &motRecherche);

                // Appel de la fonction de recherche
                T_Noeud* resultat = rechercherMot(index, motRecherche);

                if (resultat != NULL) {
                    printf("%s : \n", resultat->mot);
                    T_Position* parcour = resultat->listePositions;
                    for(int i=0;i<resultat->nbOccurences;i++){
                        printf("Occurence numero %d\n", i+1);
                        printf("Numero de ligne : %d\n", parcour->numeroLigne);
                        printf("Position dans la ligne : %d\n", parcour->ordre);
                        printf("Numero de phrase : %d\n\n", parcour->numeroPhrase);

                        parcour = parcour->suivant;
                    }
                } else {
                    printf("Mot non trouve\n");
                }
                viderBuffer();

                break;

            case '5' : ;
                char mot_cherche[100];
                printf("Entrez le nom du mot cherche : ");
                scanf(" %s", &mot_cherche);
                afficherOccurrencesMot(index, mot_cherche);
                viderBuffer();
                break;

            case '6' :
                char nom_nouv_fichier[100];
                printf("Entrez le nom du nouveau fichier : ");
                scanf(" %s", &nom_nouv_fichier);
                construireTexte(index, nom_nouv_fichier);
                viderBuffer();
                break;

            case '7' :
                printf("\n======== PROGRAMME TERMINE ========\n");
                //Faire une boucle qui free les noeuds et les positions
                free(index);
                break;

            default :
                printf("\n\nERREUR : votre choix n'est pas valide ! ");
        }
        printf("\n\n\n");
    }
    return 0;
}
