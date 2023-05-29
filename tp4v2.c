#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp4v2.h"







void parcourirFichier(T_Index* index, char* nomfichier) {
    char ligne[10000];
    int nombre_phrase = 1;
    FILE* fichier = fopen(nomfichier, "r");

    // Lire chaque ligne du fichier
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        // Découper la ligne en phrases individuelles
        char* debutPhrase = ligne;
        char* finPhrase = strstr(debutPhrase, ".");

        while (finPhrase != NULL) {
            // Retirer les espaces en début et fin de phrase
            while (*debutPhrase == ' ') {
                debutPhrase++;
            }
            int phraseLength = finPhrase - debutPhrase;
            while (phraseLength > 0 && debutPhrase[phraseLength - 1] == ' ') {
                phraseLength--;
            }

            char* phraseContent = malloc((phraseLength + 1) * sizeof(char));
            strncpy(phraseContent, debutPhrase, phraseLength);
            phraseContent[phraseLength] = '\0';

            T_Phrase* phrase = malloc(sizeof(T_Phrase));
            if (phrase != NULL) {
                phrase->contenu = phraseContent;
                phrase->numero = nombre_phrase;
                phrase->suivant = NULL;
            }

            if (index->liste_phrase == NULL) {
                index->liste_phrase = phrase;
            } else {
                T_Phrase* dernierPhrase = index->liste_phrase;
                while (dernierPhrase->suivant != NULL) {
                    dernierPhrase = dernierPhrase->suivant;
                }
                dernierPhrase->suivant = phrase;
            }

            nombre_phrase++;

            debutPhrase = finPhrase + 1;
            finPhrase = strstr(debutPhrase, ".");
        }
    }

    fclose(fichier);
}













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

    parcourirFichier(index, filename);

    int ligne = 1;
    int ordre = 1;
    int phrase = 1;
    char mot[100];  // Assumer que la longueur maximale d'un mot est de 100 caractères
    int nbMotsLus = 0;

    while (fscanf(fichier, "%s", mot) == 1) {

        if (mot[strlen(mot)-1] == '\n') {  //Pour une raison inconue ne marche pas
            mot[strlen(mot)-1] = '\0';
            ajouterOccurence(index, mot, ligne, ordre, phrase);
            nbMotsLus++;
            ligne++;
        }

        // Vérifier si le mot est un point (séparateur de phrases)
        if (mot[strlen(mot)-1] == '.') {
            mot[strlen(mot)-1] = '\0';
            ajouterOccurence(index, mot, ligne, ordre, phrase);
            nbMotsLus++;
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
// Fonction qui affiche l'index dans l'ordre alphabétique des mots
void afficherIndex(T_Index* index)
{

    // Créer un tableau pour stocker les nœuds de l'index
    T_Noeud** tableauNoeuds = (T_Noeud**)malloc(index->nbMotsDistincts * sizeof(T_Noeud*));

    // Remplir le tableau avec les nœuds de l'index
    int indexTableau = 0;
    parcoursInfixe(index->racine, tableauNoeuds, &indexTableau);

    // Afficher les mots et leurs positions dans l'index
    T_Noeud* noeud = NULL;
    T_Noeud* precedent = NULL;
    for (int i = 0; i < index->nbMotsDistincts; i++) {
        noeud = tableauNoeuds[i];
        noeud->mot[0] = toupper(noeud->mot[0]);
        //Si le mot precedent commence par une lettre différente, on modifie l'affichage
        if(precedent!= NULL && precedent->mot[0] != noeud->mot[0]){
            printf("\n %c : \n",noeud->mot[0]);
        }

        //on affiche le mot
        printf("|-- %s \n",noeud->mot);

        //On affiche toutes les positions
        T_Position* courant = noeud->listePositions;
        while (courant != NULL) {
            printf("|-- (l:%d, o:%d, p:%d) \n", courant->numeroLigne, courant->ordre, courant->numeroPhrase);
            courant = courant->suivant;
        }
        printf("|\n");
        noeud->mot[0] = tolower(noeud->mot[0]);
        precedent = tableauNoeuds[i];
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
        printf("Mot '%s': \n", mot);
        printf("Occurrences : %d\n", noeud->nbOccurences);

        T_Phrase* phrases = index->liste_phrase;
        while (phrases != NULL) {
            T_Position* position = noeud->listePositions;
            while (position != NULL) {
                if (phrases->numero == position->numeroPhrase) {
                    printf("| Ligne %d, mot %d : %s \n", phrases->numero, position->ordre, phrases->contenu);
                }
                position = position->suivant;
            }
            phrases = phrases->suivant;
        }

        printf("\n");
    }
    else {
        printf("Le mot '%s' n'a pas été trouvé dans l'index\n", mot);
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

    T_Phrase* phrases = index->liste_phrase;
    while(phrases != NULL){
        fprintf(fichier, "%s ", phrases->contenu);
        phrases = phrases->suivant;
    }

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





