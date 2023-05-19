#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED
# define MAXP 100



// Structures et types
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


//Ajout d'un élément dans la liste
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase);

//Ajout d'un mot dans l'index
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase);

//Indexer un fichier
int indexerFichier(T_Index *index, char *filename);

//Afficher les mots
void afficherIndex(T_Index *index);

//Rechercher un mot
T_Noeud* rechercherMot(T_Index index, char *mot);

//Afficher les phrase avec un certain mot
void afficherOccurencesMot(T_Index index, char *mot);

//Construire un texte
void construireTexte(T_Index index, char *filename);






typedef struct pile {
    int sommet ; // Indice de l ’ element au sommet de la pile
    int tab [ MAXP ];
} pile ;


pile creer_pile () ;
int pile_vide ( pile * p) ;
int pile_pleine ( pile * p ) ;
int empiler ( pile * p , int val ) ;
int depiler ( pile * p ) ;

void afficher_pile ( pile p ) ;


#endif
