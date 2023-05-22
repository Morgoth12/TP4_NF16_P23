#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp4.h"

/***********************************
**Ajout d'un élément dans la liste**
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

    // Cas d'insertion en tête de liste
    if (nouvelElement->numeroLigne < listeP->numeroLigne || (nouvelElement->numeroLigne == listeP->numeroLigne && nouvelElement->ordre < listeP->ordre)) {
        nouvelElement->suivant = listeP;
        return nouvelElement;
    }

    // Cas général d'insertion
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
    while (*mot) {
        *mot = tolower(*mot);
        mot++;
    }

    //Si on insère à la racine
    if(index->racine == NULL){
        index->racine = creernoeud(mot, ligne, ordre, phrase);
        return 1;
    }

    //Cas général
    T_Noeud* parcours = index->racine;
    T_Noeud* pere = NULL;
    while(parcours != NULL)
        pere = parcours;
        if(strcmp(parcours->mot,mot) == 0){
            parcours->nbOccurences++;
            ajouterPosition(parcours->listePositions, ligne, ordre, phrase);
            return 1;
        }
        if(strcmp(mot, parcours->mot) == -1){
            parcours = parcours->filsGauche;
        }
        else{
            parcours = parcours->filsDroit;
        }
    if(parcours == NULL){
        if(strcmp(pere->mot,mot)==-1){
            pere->filsGauche = creernoeud(mot, ligne, ordre, phrase);
            return 1;
        }
        else{
            pere->filsDroit = creernoeud(mot, ligne, ordre, phrase);
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
    if (fichier != NULL){       // On peut lire et ecrire dans le fichier

        char mot[100];
        int ligne =0, phrase=0, ordre=0, compteur=0;
        while (fscanf(fichier, "%s", mot) == 1) {
            if(strcmp(mot,'.') == 0){
                compteur++;
                phrase++;
                ordre = 0;
            }
            else if(strcmp(mot,'\0') == 0){
                ligne++;
            }
            else{
               ajouterOccurence(index, mot, ligne, ordre, phrase);
            }
            ordre++;
        }
    fclose(fichier); // On ferme le fichier qui a ete ouvert
    return compteur; //On renvoie le nombre de mot lu
    }
    else{
        printf("Impossible d'ouvrir le fichier %s", filename);
        return 0;
    }
}



/********************
**Afficher les mots**
********************/
void afficherIndex(T_Index* index)
{
    T_Noeud* parcour = index->racine;
    pile* P = creer_pile();
    int N = 1;
    while(P->sommet == -1 || (N == 1 && parcour != NULL)){
        if(N==1 && parcour != NULL){
            empiler(P, (parcour, 1));
            parcour = parcour->filsGauche;
        }
        else {
            (parcour, N) = depiler(P);
            if(N == 1 ){
                printf("|-- %s", parcour->mot);
                T_Position* position = parcour->listePositions;
                while(position != NULL){
                    printf("|---- (l:%d, o:%d, p:%d)", position->numeroLigne, position->ordre, position->phrase);
                    position = position->suivant;
                }
                //finir parcours + gérer l'affichage des lettres
            }
        }
    }














   /* while(parcour != NULL){
        printf("|-- %s", parcour->mot);
        T_Position* position = parcour->listePositions;
        while(position != NULL){
            printf("|---- (l:%d, o:%d, p:%d)", position->numeroLigne, position->ordre, position->phrase);
            position = position->suivant;
        }
        parcour = parcour
    }*/
}


//Rechercher un mot
T_Noeud* rechercherMot(T_Index index, char *mot)
{

}


//Afficher les phrase avec un certain mot
void afficherOccurencesMot(T_Index index, char *mot)
{

}


//Construire un texte
void construireTexte(T_Index index, char *filename)
{

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

int creernoeud(char *mot, int ligne, int ordre, int phrase)
{
    T_Noeud* nouveauNoeud = (T_Noeud*) malloc(sizeof(T_Noeud));
    nouveauNoeud->mot = mot;
    nouveauNoeud->nbOccurences = 1;
    nouveauNoeud->listePositions = NULL;
    nouveauNoeud->filsDroit = NULL;
    nouveauNoeud->filsGauche = NULL;

    ajouterPosition(nouveauNoeud->listePositions, ligne, ordre, phrase);
}

pile creer_pile()
{
    pile p;
    p.sommet = -1;
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

int empiler(pile* p, int val)
{
    if(pile_pleine(p)){
        printf("Erreur : Pile pleine \n");
        return 0;
    }
    else {
        p->sommet++;
        p->tab[p->sommet] = val ;
        return 1;
    }
}


int depiler(pile* p)
{
    if(pile_vide(p)){
        printf("Erreur : Pile vide \n");
        return INT_MAX;
    }
    else {
        p->sommet--;
        return p->tab[p->sommet+1];
    }
}
