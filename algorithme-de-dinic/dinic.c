#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "dinic.h"
// -----------------------------------------------------------------------------
// Fonctions utilitaires génériques et file d'attente FIFO d'entiers
//
// - xmalloc(size)  : alloue 'size' octets ou termine le programme en cas d'échec.
// - Queue          : structure pour gérer une file d'entiers.
//   - queue_create : crée une file de capacité 'n'.
//   - queue_empty  : teste si la file est vide.
//   - queue_push   : ajoute un entier à la fin de la file.
//   - queue_pop    : retire et renvoie l'entier en tête de la file.
//   - queue_free   : libère la mémoire associée à la file.
// -----------------------------------------------------------------------------
static void *xmalloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

typedef struct {
    int *data;
    int head, tail, capacity;
} Queue;

static Queue *queue_create(int n) {
    Queue *q = xmalloc(sizeof *q);
    q->data     = xmalloc(n * sizeof *q->data);
    q->head = q->tail = 0;
    q->capacity = n;
    return q;
}

static bool queue_empty(const Queue *q) {
    return q->head == q->tail;
}

static void queue_push(Queue *q, int v) {
    q->data[q->tail++] = v;
}

static int queue_pop(Queue *q) {
    return q->data[q->head++];
}

static void queue_free(Queue *q) {
    free(q->data);
    free(q);
}

/***********************************************************************************************************/
/* Action buildGraph :                                                                                     */
/*   - lit un fichier DIMACS et construit le réseau initial                                               */
/*   - paramètres : const char* filename (chemin du fichier DIMACS)                                       */
/*   - retourne : Graphe* initialisé (flots à 0)                                                            */
/* Fonctionnement :                                                                                        */
/*   1. Lecture de la ligne 'p' pour obtenir nombre de sommets et d'arcs                                   */
/*   2. Allocation du Graphe et initialisation des listes de successeurs                                   */
/*   3. Parcours des lignes :                                                                             */
/*      - 'n id s|t' pour identifier source et puits                                                       */
/*      - 'a u v c' pour créer un Maillon (u->v) avec capacité c                                            */
/*   4. Vérification que source et puits sont valides                                                       */
/***********************************************************************************************************/
Graphe* buildGraph(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Ouverture fichier"); exit(EXIT_FAILURE); }

    int n = 0, m = 0, src = -1, snk = -1;
    char line[256];
    (void)sscanf; // pour supprimer l'avertissement
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == 'c') continue;
        if (line[0] == 'p') {
            char tag[16];
            (void)sscanf(line, "p %15s %d %d", tag, &n, &m);
            break;
        }
    }
    if (n <= 0) { fprintf(stderr, "DIMACS invalide: nb sommets\n"); exit(EXIT_FAILURE); }

    Graphe *g = xmalloc(sizeof *g);
    g->nbSommets = n + 1;
    g->successeurs = xmalloc((n + 1) * sizeof *g->successeurs);
    for (int i = 0; i <= n; i++) g->successeurs[i] = (ListeSucc){0};

    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == 'c' || line[0] == 'p') continue;
        if (line[0] == 'n') {
            int id; char t;
            if (sscanf(line, "n %d %c", &id, &t) == 2) {
                if (t == 's') src = id;
                else if (t == 't') snk = id;
            }
        } else if (line[0] == 'a') {
            int u, v, c;
            if (sscanf(line, "a %d %d %d", &u, &v, &c) == 3) {
                Maillon *arc = xmalloc(sizeof *arc);
                *arc = (Maillon){ .SommetDest = v, .capa = c, .flot = 0, .suivant = g->successeurs[u].tete };
                g->successeurs[u].tete = arc;
                g->successeurs[u].taille++;
            }
        }
    }
    fclose(fp);
    if (src < 1 || src > n || snk < 1 || snk > n) { fprintf(stderr, "Source ou puits manquant\n"); exit(EXIT_FAILURE); }
    g->source = src; g->puits = snk;
    return g;
}

/***********************************************************************************************************/
/* Action buildResidualGraph :                                                                            */
/*   - construit le graphe résiduel à partir du réseau initial                                             */
/*   - crée pour chaque arc (u->v) deux arcs : direct (c-f) et inverse (f)                                 */
/*   - pas de pointeur reverse, l'arc inverse est cherché dynamiquement                                     */
/*   - paramètres : const Graphe* g (réseau initial)                                                      */
/*   - retourne : Graphe* résiduel                                                                         */
/***********************************************************************************************************/
Graphe* buildResidualGraph(const Graphe *g) {
    int n = g->nbSommets;
    Graphe *rg = xmalloc(sizeof *rg);
    *rg = (Graphe){ .nbSommets = n, .successeurs = xmalloc(n * sizeof *rg->successeurs),
                   .source = g->source, .puits = g->puits };
    for (int i = 0; i < n; i++) rg->successeurs[i] = (ListeSucc){0};

    for (int u = 1; u < n; u++) {
        for (Maillon *e = g->successeurs[u].tete; e; e = e->suivant) {
            int v = e->SommetDest;
            int cd = e->capa - e->flot;
            int cr = e->flot;
            Maillon *ad = xmalloc(sizeof *ad);
            *ad = (Maillon){ .SommetDest = v, .capa = cd, .flot = 0, .suivant = rg->successeurs[u].tete };
            rg->successeurs[u].tete = ad;
            rg->successeurs[u].taille++;
            Maillon *ar = xmalloc(sizeof *ar);
            *ar = (Maillon){ .SommetDest = u, .capa = cr, .flot = 0, .suivant = rg->successeurs[v].tete };
            rg->successeurs[v].tete = ar;
            rg->successeurs[v].taille++;
        }
    }
    return rg;
}

/***********************************************************************************************************/
/* Action shortestPath :                                                                                   */
/*   - trouve un plus court chemin (l'exploration en largeur) de source à puits dans le graphe résiduel                         */
/*   - paramètres : const Graphe* rg (graphe résiduel)                                                     */
/*   - retourne : Chemin* (séquence de sommets) ou NULL si pas de chemin                                   */
/***********************************************************************************************************/
Chemin* shortestPath(const Graphe *rg) {
    int n = rg->nbSommets;
    bool *vis = xmalloc(n * sizeof *vis);
    int *par = xmalloc(n * sizeof *par);
    for (int i = 0; i < n; i++) vis[i] = false;

    Queue *q = queue_create(n);
    queue_push(q, rg->source);
    vis[rg->source] = true;
    par[rg->source] = -1;
    bool found = false;
    while (!queue_empty(q)) {
        int u = queue_pop(q);
        if (u == rg->puits) { found = true; break; }
        for (Maillon *e = rg->successeurs[u].tete; e; e = e->suivant) {
            int v = e->SommetDest;
            if (!vis[v] && e->capa > 0) {
                vis[v] = true;
                par[v] = u;
                queue_push(q, v);
            }
        }
    }
    queue_free(q);
    if (!found) { free(vis); free(par); return NULL; }

    int len = 0;
    for (int v = rg->puits; v != -1; v = par[v]) len++;
    Chemin *path = xmalloc(sizeof *path);
    path->taille = len;
    path->sommets = xmalloc(len * sizeof *path->sommets);
    for (int idx = len - 1, v = rg->puits; v != -1; v = par[v], idx--) {
        path->sommets[idx] = v;
    }
    free(vis);
    free(par);
    return path;
}

/***********************************************************************************************************/
/* Action minCapacity :                                                                                    */
/*   - calcule la capacité minimale des arcs le long d'un chemin                                            */
/*   - paramètres : const Graphe* rg (graphe résiduel), const Chemin* path                                 */
/*   - retourne : int (capacité minimale)                                                                  */
/***********************************************************************************************************/
int minCapacity(const Graphe *rg, const Chemin *path) {
    int minC = INT_MAX;
    for (int i = 0; i < path->taille - 1; i++) {
        int u = path->sommets[i];
        int v = path->sommets[i+1];
        for (Maillon *e = rg->successeurs[u].tete; e; e = e->suivant) {
            if (e->SommetDest == v && e->capa < minC) {
                minC = e->capa;
            }
        }
    }
    return minC;
}

/***********************************************************************************************************/
/* Action updateResidualGraph :                                                                            */
/*   - met à jour les capacités résiduelles selon l'augmentation k                                         */
/*   - paramètres : Graphe* rg (graphe résiduel), const Chemin* path, int k (flot à ajouter)               */
/***********************************************************************************************************/
void updateResidualGraph(Graphe *rg, const Chemin *path, int k) {
    for (int i = 0; i < path->taille - 1; i++) {
        int u = path->sommets[i];
        int v = path->sommets[i+1];
        // arc direct
        for (Maillon *e = rg->successeurs[u].tete; e; e = e->suivant) {
            if (e->SommetDest == v) { e->capa -= k; break; }
        }
        // arc inverse
        for (Maillon *e = rg->successeurs[v].tete; e; e = e->suivant) {
            if (e->SommetDest == u) { e->capa += k; break; }
        }
    }
}

/***********************************************************************************************************/
/* Action updateNetworkFlow :                                                                               */
/*   - applique dans le réseau initial les flots calculés dans le graphe résiduel                            */
/*   - paramètres : const Graphe* rg (graphe résiduel), Graphe* g (réseau initial)                         */
/***********************************************************************************************************/
void updateNetworkFlow(const Graphe *rg, Graphe *g) {
    for (int u = 1; u < g->nbSommets; u++) {
        for (Maillon *e = g->successeurs[u].tete; e; e = e->suivant) {
            int init_capa = e->capa;
            for (Maillon *r = rg->successeurs[u].tete; r; r = r->suivant) {
                if (r->SommetDest == e->SommetDest) {
                    e->flot = init_capa - r->capa;
                    break;
                }
            }
        }
    }
}

/***********************************************************************************************************/
/* Action ecrireResultat :                                                                                  */
/*   - écrit le flot maximal et les flux des arcs dans un fichier texte "resultat.txt"                      */
/*   - paramètres : const Graphe* res (réseau final), int flot_total (flot maximal)                         */
/***********************************************************************************************************/
void ecrireResultat(const Graphe* res, int flot_total) {
    FILE* fout = fopen("resultat.txt", "w");
    if (!fout) {
        fprintf(stderr, "Erreur : impossible de créer le fichier resultat.txt\n");
        return;
    }
    fprintf(fout, "Flot maximal : %d\n", flot_total);
    fprintf(fout, "\nFlux sur les arcs :\n");
    for (int u = 1; u < res->nbSommets; u++) {
        for (Maillon* a = res->successeurs[u].tete; a; a = a->suivant) {
            fprintf(fout, "%d -> %d : flux %d / capacité %d\n",
                    u, a->SommetDest, a->flot, a->capa);
        }
    }
    fclose(fout);
    printf("Résultat écrit dans le fichier resultat.txt\n");
}

/***********************************************************************************************************/
/* Action freeGraph/freePath :                                                                              */
/*   - libère la mémoire allouée pour un Graphe ou un Chemin                                                */
/***********************************************************************************************************/
void freeGraph(Graphe *g) {
    if (!g) return;
    for (int u = 0; u < g->nbSommets; u++) {
        Maillon *e = g->successeurs[u].tete;
        while (e) { Maillon *n = e->suivant; free(e); e = n; }
    }
    free(g->successeurs);
    free(g);
}

void freePath(Chemin *path) {
    if (!path) return;
    free(path->sommets);
    free(path);
}

/***********************************************************************************************************/
/* main :                                                                                                   */
/*   - exécute l'algorithme de Dinic et écrit le résultat                                                   */
/*   - paramètres : argc, argv (chemin du fichier DIMACS)                                                   */
/***********************************************************************************************************/
int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "Usage: %s <DIMACS>\n", argv[0]); return EXIT_FAILURE; }
    Graphe *network = buildGraph(argv[1]);
    Graphe *resGraph = buildResidualGraph(network);

    int maxFlow = 0;
    Chemin *path;
    while ((path = shortestPath(resGraph))) {
        int k = minCapacity(resGraph, path);
        updateResidualGraph(resGraph, path, k);
        updateNetworkFlow(resGraph, network);
        maxFlow += k;
        freePath(path);
    }

    ecrireResultat(network, maxFlow);
    freeGraph(resGraph);
    freeGraph(network);
    return EXIT_SUCCESS;
}
