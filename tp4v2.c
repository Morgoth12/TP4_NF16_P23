#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp4v2.h"



// Fonction qui ajoute une position dans une liste triée de positions
T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase) {
    // Allouer un nouvel élément de position
    T_Position* nouvelElement = (T_Position*)malloc(sizeof(T_Position));
    nouvelElement->numeroLigne = ligne;
    nouvelElement->ordre = ordre;
    nouvelElement->numeroPhrase = phrase;
    nouvelElement->suivant = NULL;

    // Si la liste est vide ou si le nouvel élément doit être inséré en tête
    if (listeP == NULL || nouvelElement->numeroLigne < listeP->numeroLigne || (nouvelElement->numeroLigne == listeP->numeroLigne && nouvelElement->ordre < listeP->ordre)) {
        nouvelElement->suivant = listeP;
        return nouvelElement;
    }

    // Parcourir la liste jusqu'à l'emplacement d'insertion
    T_Position* courant = listeP;
    while (courant->suivant != NULL && (nouvelElement->numeroLigne > courant->suivant->numeroLigne || (nouvelElement->numeroLigne == courant->suivant->numeroLigne && nouvelElement->ordre > courant->suivant->ordre))) {
        courant = courant->suivant;
    }

    // Insérer le nouvel élément dans la liste
    nouvelElement->suivant = courant->suivant;
    courant->suivant = nouvelElement;

    return listeP;
}




// Fonction qui ajoute un mot dans l'index
int ajouterOccurence(T_Index* index, char* mot, int ligne, int ordre, int phrase)
{
    // Convertir le mot en minuscules pour ignorer la casse
    int i = 0;
    while (mot[i]) {
        mot[i] = tolower(mot[i]);
        i++;
    }

    // Si l'index est vide, créer un nouveau nœud pour le mot
    if (index->racine == NULL) {
        T_Noeud* nouvelElement = (T_Noeud*)malloc(sizeof(T_Noeud));
        nouvelElement->mot = strdup(mot);
        nouvelElement->nbOccurences = 1;
        nouvelElement->listePositions = ajouterPosition(NULL, ligne, ordre, phrase);
        nouvelElement->filsGauche = NULL;
        nouvelElement->filsDroit = NULL;
        index->racine = nouvelElement;
        index->nbMotsDistincts++;
        index->nbMotsTotal++;
        return 1;
    }

    // Recherche de l'emplacement d'insertion dans l'arbre
    T_Noeud* courant = index->racine;
    T_Noeud* parent = NULL;
    while (courant != NULL) {
        parent = courant;
        int comparaison = strcmp(mot, courant->mot);
        if (comparaison == 0) {
            // Le mot existe déjà dans l'index, ajouter la position à sa liste
            courant->listePositions = ajouterPosition(courant->listePositions, ligne, ordre, phrase);
            courant->nbOccurences++;
            index->nbMotsTotal++;
            return 1;
        }
        else if (comparaison < 0) {
            courant = courant->filsGauche;
        }
        else {
            courant = courant->filsDroit;
        }
    }

    // Création d'un nouveau nœud pour le mot et insertion dans l'arbre
    T_Noeud* nouvelElement = (T_Noeud*)malloc(sizeof(T_Noeud));
    nouvelElement->mot = strdup(mot);
    nouvelElement->nbOccurences = 1;
    nouvelElement->listePositions = ajouterPosition(NULL, ligne, ordre, phrase);
    nouvelElement->filsGauche = NULL;
    nouvelElement->filsDroit = NULL;

    if (strcmp(mot, parent->mot) < 0) {
        parent->filsGauche = nouvelElement;
    }
    else {
        parent->filsDroit = nouvelElement;
    }

    index->nbMotsDistincts++;
    index->nbMotsTotal++;
    return 1;
}




// Fonction qui lit le fichier et ajoute les mots dans l'index
int indexerFichier(T_Index* index, char* filename)
{
    FILE* fichier = fopen(filename, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return 0;
    }

    int ligne = 1;
    int ordre = 1;
    int phrase = 1;
    char mot[100];  // Assumer que la longueur maximale d'un mot est de 100 caractères
    int nbMotsLus = 0;

    while (fscanf(fichier, "%s", mot) == 1) {
        // Vérifier si le mot est un point (séparateur de phrases)
        if (strcmp(mot, ".") == 0) {
            phrase++;
            ordre = 1;
        }
        else {
            // Ajouter le mot dans l'index
            if (ajouterOccurence(index, mot, ligne, ordre, phrase))
                nbMotsLus++;
            ordre++;
        }
    }

    fclose(fichier);
    return nbMotsLus;
}




// Fonction qui affiche l'index dans l'ordre alphabétique des mots
void afficherIndex(T_Index* index)
{

    // Créer un tableau pour stocker les nœuds de l'index
    T_Noeud** tableauNoeuds = (T_Noeud**)malloc(index->nbMotsDistincts * sizeof(T_Noeud*));

    // Remplir le tableau avec les nœuds de l'index
    int indexTableau = 0;
    parcoursInfixe(index->racine, tableauNoeuds, &indexTableau);

    // Afficher les mots et leurs positions dans l'index
    for (int i = 0; i < index->nbMotsDistincts; i++) {
        T_Noeud* noeud = tableauNoeuds[i];
        printf("%s: ", noeud->mot);
        afficherPositions(noeud->listePositions);
        printf("\n");
    }

    // Libérer la mémoire allouée pour le tableau des nœuds
    free(tableauNoeuds);
}




// Fonction qui recherche un mot dans l'index
T_Noeud* rechercherMot(T_Index* index, char* mot) {
    // Convertir le mot en minuscules pour ignorer la casse
    int i = 0;
    while(mot[i]) {
        mot[i] = tolower(mot[i]);
        i++;
    }

    // Parcourir l'arbre à la recherche du mot
    T_Noeud* courant = index->racine;
    while (courant != NULL) {
        int comparaison = strcmp(mot, courant->mot);
        if (comparaison == 0) {
            return courant;
        } else if (comparaison < 0) {
            courant = courant->filsGauche;
        } else {
            courant = courant->filsDroit;
        }
    }

    // Le mot n'a pas été trouvé dans l'index
    return NULL;
}



// Vider le buffer (utile quand on utlise des getchar() )
void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}



// Fonction qui affiche les occurrences d'un mot dans l'index
void afficherOccurrencesMot(T_Index* index, char* mot)
{
    T_Noeud* noeud = rechercherMot(index, mot);
    if (noeud != NULL) {
        printf("Occurrences du mot '%s': ", mot);
        afficherPositions(noeud->listePositions);
        printf("\n");
    }
    else {
        printf("Le mot '%s' n'a pas été trouvé dans l'index.\n", mot);
    }
}




// Procédure qui construit le texte à partir de l'index et l'enregistre dans un fichier texte
void construireTexte(T_Index* index, char* filename)
{
    FILE* fichier = fopen(filename, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    constructionTexteAux(index->racine, fichier);

    fclose(fichier);
}







// Fonction qui affiche les positions d'un mot
void afficherPositions(T_Position* listePositions) {
    T_Position* courant = listePositions;
    while (courant != NULL) {
        printf("(%d, %d, %d) ", courant->numeroLigne, courant->ordre, courant->numeroPhrase);
        courant = courant->suivant;
    }
}





void parcoursInfixe(T_Noeud* racine, T_Noeud* tableauNoeuds[], int* indexTableau)
{
    if (racine == NULL) {
        return;
    }

    parcoursInfixe(racine->filsGauche, tableauNoeuds, indexTableau);

    tableauNoeuds[*indexTableau] = racine;
    (*indexTableau)++;

    parcoursInfixe(racine->filsDroit, tableauNoeuds, indexTableau);
}


// Fonction auxiliaire pour construire le texte à partir de l'index
void constructionTexteAux(T_Noeud* noeud, FILE* fichier)
{
    if (noeud != NULL) {
        constructionTexteAux(noeud->filsGauche, fichier);
        // Écrire le mot dans le fichier
        fprintf(fichier, "%s ", noeud->mot);
        constructionTexteAux(noeud->filsDroit, fichier);
    }
}


