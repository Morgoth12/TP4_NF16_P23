#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structures de données

typedef struct Position {
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct Position* suivant;
} T_Position;

typedef struct Noeud {
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    struct Noeud* filsGauche;
    struct Noeud* filsDroit;
} T_Noeud;

typedef struct Index {
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
} T_Index;

// Fonctions de base

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
    if (nouvelElement->numeroLigne < listeP->numeroLigne
        || (nouvelElement->numeroLigne == listeP->numeroLigne
            && nouvelElement->ordre < listeP->ordre)) {
        nouvelElement->suivant = listeP;
        return nouvelElement;
    }

    // Cas général d'insertion
    T_Position* courant = listeP;
    while (courant->suivant != NULL
           && (nouvelElement->numeroLigne > courant->suivant->numeroLigne
               || (nouvelElement->numeroLigne == courant->suivant->numeroLigne
                   && nouvelElement->ordre >= courant->suivant->ordre))) {
        courant = courant->suivant;
    }
    nouvelElement->suivant = courant->suivant;
    courant->suivant = nouvelElement;

    return listeP;
}

int ajouterOccurence(T_Index* index, char* mot, int ligne, int ordre, int phrase) {
    // Convertir le mot en minuscules
    int i;
    for (i = 0; mot[i] != '\0'; i++) {
        mot[i] = tolower(mot[i]);
    }

    // Parcourir l'arbre à la recherche du noeud correspondant
    T_Noeud** courant = &(index->racine);
    while (*courant != NULL) {
        int cmp = strcmp(mot, (*courant)->mot);
        if (cmp == 0) {
            // Le mot est déjà présent dans l'arbre
            (*courant)->nbOccurences++;
            (*courant)->listePositions = ajouterPosition((*courant)->listePositions, ligne, ordre, phrase);
            return 1;
        } else if (cmp < 0) {
            // Le mot est plus petit que le noeud courant, on descend dans le fils gauche
            courant = &((*courant)->filsGauche);
        } else {
            // Le mot est plus grand que le noeud courant, on descend dans le fils droit
            courant = &((*courant)->f
