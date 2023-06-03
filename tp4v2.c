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

        index->nbMotsDistincts++;   //On incrémente le nombre de mots lus
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


    index->nbMotsDistincts++;   //On incrémente le nombre de mots lus
    index->nbMotsTotal++;
    return 1;
}




// Fonction qui lit le fichier et ajoute les mots dans l'index
int indexerFichier(T_Index* index, char* filename) {
    FILE* fichier = fopen(filename, "r");   //On ouvre Le fichier en mode lecture
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return 0;
    }

    int ligne = 1;
    int ordre = 1;
    int phrase = 1;
    char mot[100];
    int nbMotsLus = 0;
    char contenu[1000];
    contenu[0] = '\0';  // Initialiser à une chaîne vide
    char ligneCourante[1000];
    ligneCourante[0] = '\0';  // Initialiser à une chaîne vide

    while (fgets(ligneCourante, sizeof(ligneCourante), fichier) != NULL) {
        char* token = strtok(ligneCourante, " \t\n");  // On divise la ligne en mots

        while (token != NULL) {
            strcpy(mot, token);

            // On vérifie si le mot est un point (séparateur de phrases)
            if (mot[strlen(mot) - 1] == '.') {
                mot[strlen(mot) - 1] = '\0';
                ajouterOccurence(index, mot, ligne, ordre, phrase);
                nbMotsLus++;
                phrase++;
                strcat(contenu, mot);

                T_Phrase* nouvellePhrase = malloc(sizeof(T_Phrase));    //On crée une nouvelle phrase afin de pouvoir l'ajouter dans la liste de phrase de l'index
                if (nouvellePhrase != NULL) {
                    nouvellePhrase->contenu = strdup(contenu);      //On met les mots que l'on a scanné à partir du dernier point dans la phrase
                    nouvellePhrase->numero = phrase;
                    nouvellePhrase->suivant = NULL;
                }

                if (index->liste_phrase == NULL) {      //On ajoute la phrase dans la liste de phrase du fichier
                    index->liste_phrase = nouvellePhrase;
                } else {
                    T_Phrase* dernierPhrase = index->liste_phrase;
                    while (dernierPhrase->suivant != NULL) {
                        dernierPhrase = dernierPhrase->suivant;
                    }
                    dernierPhrase->suivant = nouvellePhrase;
                }

                // On réinitialise le contenu pour la prochaine phrase
                contenu[0] = '\0';
                ordre++;    //On augmente l'ordre puisqu'on passe au mot suivant

            } else {
                // Ajouter le mot dans l'index
                if (ajouterOccurence(index, mot, ligne, ordre, phrase))
                    nbMotsLus++;
                ordre++;    //On augmente l'ordre puisqu'on passe au mot suivant
                strcat(contenu, mot);  // Ajouter le mot au contenu de la phrase
                strcat(contenu, " ");  // Ajouter un espace après le mot
            }

            token = strtok(NULL, " \t\n");  // Passer au mot suivant
        }
        strcat(contenu, "\n");  //On ajoute un retour à la ligne à la phrase actuelle pour pouvoirle marquer lorsqu'on les affichera plus tard
        ligne++;    //On augmnte le nombre de lignes puisqu'on passe à la ligne suivante
        ordre = 1;  //On réinitialise l'ordre puisqu'on passe à la ligne suivante
    }

    fclose(fichier);    //On ferme le fichier
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
    T_Noeud* noeud = NULL;
    T_Noeud* precedent = NULL;
    for (int i = 0; i < index->nbMotsDistincts; i++) {
        noeud = tableauNoeuds[i];
        //Si le mot precedent commence par une lettre différente, on modifie l'affichage
        if(precedent == NULL ||  precedent->mot[0] != noeud->mot[0]){
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
        //noeud->mot[0] = tolower(noeud->mot[0]);
        precedent = tableauNoeuds[i];
    }

    // On libère la mémoire allouée pour le tableau des nœuds
    free(tableauNoeuds);
}




// Fonction qui recherche un mot dans l'index
T_Noeud* rechercherMot(T_Index* index, char* mot) {
    // On converti le mot en minuscules pour ignorer la casse
    int i = 0;
    while(mot[i]) {
        mot[i] = tolower(mot[i]);
        i++;
    }

    // On parcourt l'arbre à la recherche du mot
    T_Noeud* courant = index->racine;
    while (courant != NULL) {
        int comparaison = strcmp(mot, courant->mot);
        if (comparaison == 0) {
            return courant;     //Si on a trouvé le mot on le renvois
        } else if (comparaison < 0) {
            courant = courant->filsGauche;  //Si le mot est avant dans l'ordre alphabétique on passe à gauche
        } else {
            courant = courant->filsDroit;   //Si le mot est après dans l'ordre alphabétique on passe à droite
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
    T_Noeud* noeud = rechercherMot(index, mot);     //On cherche le noed du mot que l'on veut afficher afin d'avoir accès à ses informations
    if (noeud != NULL) {
        printf("Mot '%s': \n", mot);
        printf("Occurrences : %d\n", noeud->nbOccurences);

        T_Phrase* phrases = index->liste_phrase;    //On parcours les phrases
        while (phrases != NULL) {
            T_Position* position = noeud->listePositions;
            while (position != NULL) {      //On parcours les différentes positions du mot
                if (phrases->numero == position->numeroPhrase) {    //On regarde si la phrase comporte le mot

                    char* content = strdup(phrases->contenu);
                    content[0]=toupper(content[0]);     //On met une majuscule à la fin de la phrase
                    for (int i = 0; i < strlen(content); i++) {
                        if (content[i] == '\n') {
                            content[i] = ' ';  // On remplace les séquences de nouvelle ligne par des espaces, ceci n'est pas obligatoire mais nous avons préféré le faire pour un détail d'affichage
                        }
                    }

                    printf("| Ligne %d, mot %d : %s \n", phrases->numero, position->ordre, content);    //On affiche les information de l'occurence ainsi que la phrase
                }
                position = position->suivant;   //On passe à la position suivante
            }
            phrases = phrases->suivant;     //On passe à la phrase suivante
        }

        printf("\n");
    }
    else {
        printf("Le mot '%s' n'a pas ete trouve dans l'index\n", mot);
    }
}





// Procédure qui construit le texte à partir de l'index et l'enregistre dans un fichier texte
void construireTexte(T_Index* index, char* filename)
{
    FILE* fichier = fopen(filename, "w");   //On crée le fichier
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    T_Phrase* phrases = index->liste_phrase;    //On va parcourir les phrases
    while(phrases != NULL){
        phrases->contenu[0] = toupper(phrases->contenu[0]);     //On met une majuscule au début de la phrase
        fprintf(fichier, "%s", phrases->contenu);       //On écrit la phrase dans le fichier
        fprintf(fichier, ". ");     //On met un point à la fin de la phrase
        phrases = phrases->suivant;     //On passe à la phrase suivante
    }

    fclose(fichier);    //On ferme le fichier
}







// Fonction qui affiche les positions d'un mot
void afficherPositions(T_Position* listePositions) {
    T_Position* courant = listePositions;
    while (courant != NULL) {   //On parcours les positions du mots
        printf("(%d, %d, %d) ", courant->numeroLigne, courant->ordre, courant->numeroPhrase);   //On affiche les informations du mot
        courant = courant->suivant;
    }
}





void parcoursInfixe(T_Noeud* racine, T_Noeud* tableauNoeuds[], int* indexTableau)
{
    if (racine == NULL) {   //Si l'abre est vide on retourne NULL
        return;
    }

    parcoursInfixe(racine->filsGauche, tableauNoeuds, indexTableau);        //On fais un parcours infixe classique

    tableauNoeuds[*indexTableau] = racine;
    (*indexTableau)++;

    parcoursInfixe(racine->filsDroit, tableauNoeuds, indexTableau);
}





