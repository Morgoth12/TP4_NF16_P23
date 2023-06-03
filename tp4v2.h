#ifndef tp4v2_H_INCLUDED
#define tp4v2_H_INCLUDED
#define MAXP 200



// Structures et types

// Structure de données pour la position d'un mot dans le texte
typedef struct Position {
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct Position* suivant;
} T_Position;

// Structure de données pour un nœud de l'arbre binaire de recherche
typedef struct Noeud {
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    struct Noeud* filsGauche;
    struct Noeud* filsDroit;
} T_Noeud;

typedef struct Phrase {
    char* contenu;
    int numero;
    struct Phrase* suivant;
} T_Phrase;

// Structure de données pour l'index
typedef struct Index {
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
    T_Phrase* liste_phrase;
} T_Index;







// Déclaration des fonctions
T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase);
int ajouterOccurrence(T_Index* index, char* mot, int ligne, int ordre, int phrase);
int indexerFichier(T_Index* index, char* filename);
void afficherIndex(T_Index* index);
T_Noeud* rechercherMot(T_Index* index, char* mot);
void afficherOccurrencesMot(T_Index* index, char* mot);
void construireTexte(T_Index* index, char* filename);





#endif
