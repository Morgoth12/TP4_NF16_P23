# TP4_NF16_P23

Introduction
Dans ce TP, nous utiliserons les arbres binaires de recherche pour implémenter un exemple d'indexation et de
recherche sur un fichier contenant un texte quelconque.
L'arbre binaire de recherche contiendra tous les mots présents dans le texte à indexer :
- Chaque nœud de l'arbre contient un mot, ainsi que la liste de ses positions dans le texte (les positions seront
donc gérées par une liste simplement chainée).
- Une position correspond au numéro de ligne dans laquelle se trouve le mot, son « ordre » dans la ligne (1
pour le premier mot de la ligne, 2 pour le deuxième mot, …etc.), ainsi que le numéro de la phrase dans
laquelle il se trouve (1ère phrase du texte, 2ème, 3ème
, …etc.).
Dans le cadre de ce TP, on se contentera d’indexer des textes qui ne contiennent que des lettres majuscules,
des minuscules et des points (séparateur de phrases). Tout autre caractère ne sera pas pris en considération.
A. Structures de données
Implémenter les structures de données suivantes :
• La structure Position (dont on définira le type synonyme T_Position) qui comporte les champs :
- numeroLigne de type int
- ordre de type int
- numeroPhrase de type int
- suivant de type struct Position*
• La structure Noeud (dont on définira le type synonyme T_Noeud) qui représente un nœud de l’ABR.
Elle comporte les champs :
- mot de type char*
- nbOccurences de type int
- listePositions de type T_Position*
- filsGauche de type struct Noeud*
- filsDroit de type struct Noeud*
• La structure Index (dont on définira le type synonyme T_Index) qui représente l’ABR dans lequel
sont stockés les mots. Cette structure comporte les champs :
- racine de type T_Noeud*
- nbMotsDistincts de type int
- nbMotsTotal de type int
NF16 - TP 4 – Les Arbres Binaires de Recherche
P23 – NF16 – TP 4 ABR – Indexation de texte 2
B. Fonctions de base
1. Implémenter une fonction qui permet d’ajouter un élément dans une liste de positions triées. Les positions
d’une liste doivent être uniques et ordonnées dans l’ordre croissant du numéro de ligne et de l’ordre du
mot dans la ligne. Cette fonction renvoie un pointeur vers le premier élément de la liste modifiée :
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase)
2. Implémenter une fonction qui permet d’ajouter un mot dans l'index en respectant les règles d'insertion
dans les ABR. Elle renvoie 1 en cas de succès, 0 sinon.
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase)
Remarque : On se basera sur l'ordre lexicographique pour déterminer la position d'un nœud dans l'arbre. Par
exemple : avion < bagage < bateau < cabane < cabine <voiture.
Il faudra ignorer la casse des mots (lettres minuscules et capitales) : « voiture », « Voiture » et « VoiTURE » doivent
être considérés comme étant le même mot
.
3. Ecrire une fonction qui permet d’indexer un fichier texte (chargement d’un fichier). i.e. lit le fichier et
ajoute tous les mots qu’il contient dans un index. Elle renvoie le nombre de mots lus.
int indexerFichier(T_Index *index, char *filename)
4. Ecrire une procédure qui affiche la liste des mots classés par ordre alphabétique.
void afficherIndex(T_Index index)
Attention : votre affichage devra être conforme à l’affichage ci-dessus
5. Ecrire une fonction qui recherche un mot dans un index (renvoie NULL si non trouvé) :
T_Noeud* rechercherMot(T_Index index, char *mot)
Figure 1 Exemple d'affichage pour l'index
NF16 - TP 4 – Les Arbres Binaires de Recherche
P23 – NF16 – TP 4 ABR – Indexation de texte 3
6. Ecrire une procédure qui permet d’afficher l’ensemble des phrases dans lesquelles se trouve un mot.
void afficherOccurencesMot(T_Index index, char *mot)
Indications :
- Vous êtes libre de procéder de la façon dont vous souhaitez pour faire cette fonction, mais essayez d’optimiser
votre algorithme.
- Si vous voulez vous pouvez ajouter des structures supplémentaires pour vous aider à améliorer votre
algorithme. Vous pouvez même modifier les structures demandées plus haut.
- Par exemple, vous pouvez décider d’ajouter un second index pour trouver plus rapidement les phrases.
7. Ecrire une procédure qui permet de construire un texte à partir de son index. Le texte résultat sera
ensuite enregistré dans un fichier texte.
void construireTexte(T_Index index, char *filename)
Indications :
- Vous êtes libre de procéder de la façon dont vous souhaitez pour faire cette fonction, mais essayez d’optimiser
votre algorithme.
- Si vous voulez vous pouvez ajouter des structures supplémentaires pour vous aider à améliorer votre
algorithme. Vous pouvez même modifier les structures demandées plus haut.
- Une utilisation d’une structure supplémentaire intermédiaire de type liste de listes, qui sera vide au départ et
qui contiendra la liste des phrases triées par ordre, et chaque élément de cette liste (une phrase) sera luimême une liste dont les maillons seront les mots de la phrase dans l’ordre, pourra vous aider dans votre
algorithme de construction de texte
