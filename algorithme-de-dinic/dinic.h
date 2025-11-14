#ifndef DINIC_H
#define DINIC_H

#include <stdbool.h>

// -----------------------------------------------------------------------------
// Structures de données pour l'algorithme de Dinic
// -----------------------------------------------------------------------------

// Maillon de la liste chaînée d'arcs sortants
// - SommetDest : numéro du sommet de destination
// - capa       : capacité résiduelle de l'arc
// - flot       : flot courant sur l'arc
// - suivant    : pointeur vers le prochain maillon de la liste

typedef struct maillon {
    int SommetDest;
    int capa;
    int flot;
    struct maillon *suivant;
} Maillon;

// Liste chaînée de maillons pour les successeurs d'un sommet
// - tete   : premier maillon de la liste
// - taille : nombre d'arcs sortants

typedef struct listeSucc {
    Maillon *tete;
    int      taille;
} ListeSucc;

// Chemin de source à puits dans le graphe résiduel
// - sommets : tableau dynamique d'indices de sommets
// - taille  : nombre de sommets dans le chemin

typedef struct chemin {
    int *sommets;
    int  taille;
} Chemin;

// Graphe orienté (réseau initial ou résiduel)
// - nbSommets    : nombre total de sommets
// - successeurs  : tableau de listes de successeurs
// - source, puits: indices de la source et du puits

typedef struct graphe {
    int       nbSommets;
    ListeSucc *successeurs;
    int       source;
    int       puits;
} Graphe;

// -----------------------------------------------------------------------------
// Prototypes des fonctions
// -----------------------------------------------------------------------------

/***********************************************************************************************************/
/* Action buildGraph :                                                                                     */
/*   - lit un fichier DIMACS et construit le réseau initial                                               */
/*   - paramètres : const char* filename                                                                 */
/*   - retourne    : Graphe* avec flots initialisés à 0                                                   */
/***********************************************************************************************************/
Graphe* buildGraph(const char *filename);

/***********************************************************************************************************/
/* Action buildResidualGraph :                                                                            */
/*   - construit le graphe résiduel à partir du réseau initial                                             */
/*   - paramètres : const Graphe* g                                                                      */
/*   - retourne    : Graphe* résiduel                                                                     */
/***********************************************************************************************************/
Graphe* buildResidualGraph(const Graphe *g);

/***********************************************************************************************************/
/* Action shortestPath :                                                                                   */
/*   - recherche BFS du plus court chemin de source à puits                                                */
/*   - paramètres : const Graphe* rg                                                                      */
/*   - retourne    : Chemin* ou NULL si pas de chemin                                                      */
/***********************************************************************************************************/
Chemin* shortestPath(const Graphe *rg);

/***********************************************************************************************************/
/* Action minCapacity :                                                                                    */
/*   - calcule la capacité minimale sur un chemin                                                          */
/*   - paramètres : const Graphe* rg, const Chemin* chemin                                                */
/*   - retourne    : int (capacité minimale)                                                              */
/***********************************************************************************************************/
int minCapacity(const Graphe *rg, const Chemin *chemin);

/***********************************************************************************************************/
/* Action updateResidualGraph :                                                                            */
/*   - met à jour les capacités résiduelles selon l'augmentation k                                          */
/*   - paramètres : Graphe* rg, const Chemin* chemin, int k                                                 */
/***********************************************************************************************************/
void updateResidualGraph(Graphe *rg, const Chemin *chemin, int k);

/***********************************************************************************************************/
/* Action updateNetworkFlow :                                                                               */
/*   - applique les flots du graphe résiduel dans le réseau initial                                        */
/*   - paramètres : const Graphe* rg, Graphe* g                                                            */
/***********************************************************************************************************/
void updateNetworkFlow(const Graphe *rg, Graphe *g);

/***********************************************************************************************************/
/* Action ecrireResultat :                                                                                  */
/*   - écrit le flot maximal et les flux des arcs dans "resultat.txt"                                      */
/*   - paramètres : const Graphe* res, int flot_total                                                       */
/***********************************************************************************************************/
void ecrireResultat(const Graphe *res, int flot_total);

/***********************************************************************************************************/
/* Action freeGraph :                                                                                       */
/*   - libère la mémoire d'un Graphe                                                                        */
/*   - paramètres : Graphe* g                                                                              */
/***********************************************************************************************************/
void freeGraph(Graphe *g);

/***********************************************************************************************************/
/* Action freePath :                                                                                        */
/*   - libère la mémoire d'un Chemin                                                                        */
/*   - paramètres : Chemin* chemin                                                                          */
/***********************************************************************************************************/
void freePath(Chemin *chemin);

#endif // DINIC_H
