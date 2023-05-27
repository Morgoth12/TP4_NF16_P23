#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp4.h"

/***********************************
**Ajout d'un element dans la liste**
***********************************/
T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase) {

    T_Position* nouvelElement = (T_Position*) malloc(sizeof(T_Position));
    nouvelElement->numeroLigne = ligne;
    nouvelElement->ordre = ordre;
    nouvelElement->numeroPhrase = phrase;
    nouvelElement->suivant = NULL;

    // Cas de la liste vide
    if (listeP == NULL) {
        return nouvelElement;
    }

    // Cas d'insertion en t�te de liste
    if (nouvelElement->numeroLigne < listeP->numeroLigne || (nouvelElement->numeroLigne == listeP->numeroLigne && nouvelElement->ordre < listeP->ordre)) {
        nouvelElement->suivant = listeP;
        return nouvelElement;
    }

    // Cas g�n�ral d'insertion
    T_Position* courant = listeP;
    while (courant->suivant != NULL && (nouvelElement->numeroLigne > courant->suivant->numeroLigne || (nouvelElement->numeroLigne == courant->suivant->numeroLigne && nouvelElement->ordre >= courant->suivant->ordre))) {
        courant = courant->suivant;
    }
    nouvelElement->suivant = courant->suivant;
    courant->suivant = nouvelElement;

    return listeP;
}

/******************************
**Ajout d'un mot dans l'index**
******************************/
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase)
{
    printf("On a recu ce mot : %s \n",mot);

        //S'il n'y a pas encore de mots, on insere a la racine
    if(index->racine == NULL){
        T_Noeud* nouveauNoeud = creernoeud(mot, ligne, ordre, phrase);
        nouveauNoeud->mot = mot;
        index->racine = nouveauNoeud;
        index->nbMotsDistincts++;
        index->nbMotsTotal++;
        return 1;
    }

    printf("mot a la racine : %s \n", index->racine->mot);

    //Cas general
    T_Noeud* parcours = index->racine;
    T_Noeud* pere = NULL;

    printf("mot noeud courant : %s \n",parcours->mot);

    while(parcours != NULL){
        printf("On cherche ou mettre le mot ! \n");
        printf("On compare ce mot : %s et ce mot : %s \n", mot, parcours->mot);
        pere = parcours;
        if(strcmp(parcours->mot,mot) == 0){
            printf("Les mots sont identiques, on ajoute une occurence! \n");
            parcours->nbOccurences++;
            index->nbMotsTotal++;
            ajouterPosition(parcours->listePositions, ligne, ordre, phrase);
            return 1;
        }
        if(strcmp(mot, parcours->mot) == -1){
            printf("Le mot est avant dans l'alphabet donc on va voir le fils gauche\n");
            parcours = parcours->filsGauche;
        }
        else{
            printf("Le mot est apres dans l'alphabet donc on va voir le fils droit! \n");
            parcours = parcours->filsDroit;
        }
    }
    if (parcours == NULL) {
        printf("On ajoute le mot ! \n");
        T_Noeud* nouveauNoeud = creernoeud(mot, ligne, ordre, phrase);
        index->nbMotsDistincts++;
        index->nbMotsTotal++;
        if (strcmp(pere->mot, mot) == -1) {
            pere->filsGauche = nouveauNoeud;
            return 1;
        }
        else {
            pere->filsDroit = nouveauNoeud;
            return 1;
        }
    }
    return 0;
}


/*********************
**Indexer un fichier**
*********************/
int indexerFichier(T_Index *index, char *filename)
{
    //Ouvrir un fichier et en lire son contenu
    FILE* fichier = NULL;
    fichier = fopen(filename, "r");
    if (fichier){       // Si le fichier existe et n'est pas vide, on le lit

        char mot[100] = "";
        int ligne =0, phrase=0, ordre=0, compteur=0;
        while (fscanf(fichier, "%s", mot) == 1) {
            if(mot[strlen(mot)-1] == '.'){
                printf("Nouvelle phrase : On reinitialise l'ordre dans la phrase et on incremente le nombre de phrases ! \n");
                compteur++;
                phrase++;
                ordre = 0;
            }
            else if(mot[strlen(mot)-1] =='\n'){
                printf("Fin de ligne, on incremente le nombre de lignes ! \n");
                ligne++;
            }
            else{
                //On ajoute le mot dans l'index
                ajouterOccurence(index, mot, ligne, ordre, phrase);
                printf("Mot ajoute : %s \n",mot);
            }
            ordre++;
        }
    fclose(fichier); // On ferme le fichier qui a ete ouvert
    return compteur; //On renvoie le nombre de mot lu
    }
    else{
        //Sinon, on retourne une erreur
        printf("Impossible d'ouvrir le fichier %s", filename);
        return 0;
    }
}



/********************
**Afficher les mots**
********************/
void afficherIndex(T_Index* index)
{
    if (index->racine == NULL) {
        return;
    }

    pile* P = creer_pile();
    empiler(P, index->racine);

    while (!pile_vide(P)) {
        T_Noeud* courant = depiler(P);

        printf("|-- %s", courant->mot);
        T_Position* position = courant->listePositions;
        while (position != NULL) {
            printf("|---- (l:%d, o:%d, p:%d)", position->numeroLigne, position->ordre, position->numeroPhrase);
            position = position->suivant;
        }

        if (courant->filsDroit != NULL) {
            empiler(P, courant->filsDroit);
        }
        if (courant->filsGauche != NULL) {
            empiler(P, courant->filsGauche);
        }
    }
}




/********************
**Rechercher un mot**
********************/
T_Noeud* rechercherMot(T_Index* index, char *mot)
{
    T_Noeud* noeud = index->racine;

    if (noeud == NULL) {
        return NULL;
    }

    int comparaison = strcmp(noeud->mot, mot);
    if (comparaison == 0) {
        return noeud;  // Mot trouv�
    } else if (comparaison > 0) {
        return rechercherMot(noeud->filsGauche, mot);  // Recherche dans le sous-arbre gauche
    } else {
        return rechercherMot(noeud->filsDroit, mot);  // Recherche dans le sous-arbre droit
    }
}



/******************************************
**Afficher les phrase avec un certain mot**
******************************************/
void afficherOccurencesMot(T_Index* index, char *mot)
{

}

/**********************
**Construire un texte**
**********************/
void construireTexte(T_Index* index, char *filename)
{
    FILE* fichier = fopen(filename, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    int* pileLignes = malloc(index.nbMotsTotal * sizeof(int));
    int* pileOrdres = malloc(index.nbMotsTotal * sizeof(int));
    int sommetLignes = -1;
    int sommetOrdres = -1;

    T_Noeud* courant = index.racine;
    while (courant != NULL || sommetLignes != -1) {
        if (courant != NULL) {
            empiler(pileLignes, &sommetLignes, courant->listePositions->numeroLigne);
            empiler(pileOrdres, &sommetOrdres, courant->listePositions->ordre);
            courant = courant->filsGauche;
        } else {
            int ligne = depiler(pileLignes, &sommetLignes);
            int ordre = depiler(pileOrdres, &sommetOrdres);
            fprintf(fichier, "(%d,%d) ", ligne, ordre);
            courant = rechercherMot(index, ".");
            if (courant != NULL) {
                T_Position* positionCourante = courant->listePositions;
                while (positionCourante != NULL && (positionCourante->numeroLigne != ligne || positionCourante->ordre != ordre)) {
                    positionCourante = positionCourante->suivant;
                }
                if (positionCourante != NULL) {
                    courant = rechercherMot(index, positionCourante->suivant->mot);
                }
            }
        }
    }
}



/* *********************
 * Fonctions utilitaires
 ********************* */
// Vider le buffer (utile quand on utlise des getchar() )
void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}




//Creer noeud

T_Noeud* creernoeud(char *mot, int ligne, int ordre, int phrase)
{
    printf("Reception de ce mot : %s que l'on veut ajouter dans le noeud \n",mot);
    T_Noeud* nouveauNoeud = (T_Noeud*) malloc(sizeof(T_Noeud));
    nouveauNoeud->mot = mot;
    nouveauNoeud->nbOccurences = 1;
    nouveauNoeud->listePositions = NULL;
    nouveauNoeud->filsDroit = NULL;
    nouveauNoeud->filsGauche = NULL;
    ajouterPosition(nouveauNoeud->listePositions, ligne, ordre, phrase);
    return nouveauNoeud;
}

pile* creer_pile()
{
    pile* p = malloc(sizeof(pile));
    p->sommet = -1;
    memset(p->taille, 0, sizeof(p->taille)); // Initialise le tableau à zéro
    return p;
}


int pile_vide(pile* p)
{
    return(p->sommet == -1);
}

int pile_pleine(pile* p)
{
    return(p->sommet == MAXP -1);
}

int empiler(pile* p, T_Noeud* val)
{
    if (pile_pleine(p)) {
        printf("Erreur : Pile pleine\n");
        return 0;
    } else {
        p->sommet++;
        p->taille[p->sommet] = val;
        return 1;
    }
}



T_Noeud* depiler(pile* p)
{
    if (pile_vide(p)) {
        printf("Erreur : Pile vide\n");
        return NULL;
    } else {
        T_Noeud* n = p->taille[p->sommet];
        p->sommet--;
        return n;
    }
}


//peut être à remplacer par ça, a tester :
/*
int depiler(int* pile, int* sommet) {
int element = pile[*sommet];
(*sommet)--;
return element;
}

void empiler(int* pile, int* sommet, int element) {
(*sommet)++;
pile[*sommet] = element;
}
*/

