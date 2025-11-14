#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************
 *  Définitions des constantes
 *******************************************************/
#define TAILLE_PLATEAU 19
/*
 * Structure : Pion
 * ----------------
 *  - Rôle : représenter un pion sur le plateau.
 *  - Attribut :
 *      symbole : 'X', 'O' ou '.' (pour une case vide)
 *
 */
typedef struct {
    char symbole; // 'X', 'O', ou '.' pour une case vide
} Pion;

/*
 * Structure : Plateau
 * -------------------
 *  - Rôle : représenter l'état complet d'un plateau de Penté
 *  - Attributs :
 *      plateau      : tableau 19×19 de Pion
 *      prises_joueur_X : nombre de pions capturés par le joueur X
 *      prises_joueur_O : nombre de pions capturés par le joueur O
 */
typedef struct {
    Pion plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];
    int prises_joueur_X;
    int prises_joueur_O;
} Plateau;


/*
 * -------------------------------------------------------
 * Fonction: position_valide
 * -------------------------------------------------------
 * Nature       : Fonction booléenne
 * But          : Vérifier si (ligne, colonne) est dans
 *                les limites du plateau (0..18)
 * Données      : ligne (int), colonne (int)
 * Résultat     : retourne true si la position est valide,
 *                false sinon
 * -------------------------------------------------------
 */
bool position_valide(int ligne, int colonne) {
    return (ligne >= 0 && ligne < TAILLE_PLATEAU &&
            colonne >= 0 && colonne < TAILLE_PLATEAU);
}


/*
 * -------------------------------------------------------
 * action : initialiser_plateau
 * -------------------------------------------------------
 * But          : Mettre tout le plateau à '.' (cases vides)
 *                et réinitialiser les compteurs de prises.
 * Données      : p (pointeur vers Plateau)
 * Variables locales : i, j (pour parcourir le plateau)
 * -------------------------------------------------------
 */
void initialiser_plateau(Plateau *p) {
    for (int i = 0; i < TAILLE_PLATEAU; i++) {
        for (int j = 0; j < TAILLE_PLATEAU; j++) {
            p->plateau[i][j].symbole = '.';  // case vide
        }
    }
    p->prises_joueur_X = 0;
    p->prises_joueur_O = 0;
}
/*
 * -------------------------------------------------------
 * AJOUT pour match nul : fonction est_plein
 * -------------------------------------------------------
 * Nature       : Fonction booléenne
 * But          : Vérifier si le plateau est plein
 *                (aucune case '.' disponible).
 * Données      : p (pointeur constant vers Plateau)
 * Résultat     : true si le plateau est plein,
 *                false sinon
 * Variables locales : i, j (pour parcourir le plateau)
 * -------------------------------------------------------
 */

bool est_plein(const Plateau *p) {  //--- AJOUT pour match nul ---
    for (int i = 0; i < TAILLE_PLATEAU; i++) {
        for (int j = 0; j < TAILLE_PLATEAU; j++) {
            if (p->plateau[i][j].symbole == '.') {
                return false;
            }
        }
    }
    return true;
}
/*
 * -------------------------------------------------------
 * action : afficher_plateau
 * -------------------------------------------------------
 * But          : Affiche l'état du plateau sur la console
 *                avec :
 *                  - les numéros de lignes (1..19)
 *                  - les numéros de colonnes (1..19)
 *                  - les symboles 'X', 'O' ou '.'
 *                  - le nombre de prises pour X et O
 * Données      : p (pointeur constant vers Plateau)
 * Variables locales : i, j (pour parcourir)
 * -------------------------------------------------------
 */
void afficher_plateau(const Plateau *p) {
    // En-tête de colonnes
    printf("\n    ");
    for (int i = 1; i <= TAILLE_PLATEAU; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    // Lignes du plateau
    for (int i = 0; i < TAILLE_PLATEAU; i++) {
        // Numéro de ligne
        printf("%3d ", i + 1);
        // Cases de la ligne i
        for (int j = 0; j < TAILLE_PLATEAU; j++) {
            printf(" %c ", p->plateau[i][j].symbole);
        }
        printf("\n");
    }

    // Affichage des prises
    printf("\n");
    printf("Prises du joueur X : %d\n", p->prises_joueur_X);
    printf("Prises du joueur O : %d\n\n", p->prises_joueur_O);
}


/*
 * -------------------------------------------------------
 * fonction: placer_pion           
 * -------------------------------------------------------
 * Nature       : Fonction booléenne
 * But          : Placer un pion (symbole) dans la case
 *                (ligne,colonne), si celle-ci est libre
 *                et dans les limites.
 *                Retourne true si placement réussi,
 *                false sinon.
 * Données      : p (pointeur vers Plateau),
 *                ligne, colonne :entiers,
 *                symbole:chaine de caracteres (char = 'X' ou 'O')
 * Résultat:     - retourne true si ok, false si invalid
 * -------------------------------------------------------
 */
bool placer_pion(Plateau *p, int ligne, int colonne, char symbole) {
    // Vérifie la validité et si la case est vide
    if (!position_valide(ligne, colonne) ||
        p->plateau[ligne][colonne].symbole != '.') {
        return false;
    }
    // Place le symbole
    p->plateau[ligne][colonne].symbole = symbole;
    return true;
}


/*
 * -------------------------------------------------------
 * fonction: verifier_alignement 
 * -------------------------------------------------------
 * Nature       : Fonction booléenne
 * But          : Vérifier si le pion posé à (ligne,colonne)
 *                crée un alignement d'au moins 5 pions
 *                (horizontal, vertical, diagonale, etc.)
 * Données      : p (pointeur constant vers Plateau),
 *                ligne, colonne:entiers
 *                symbole:chaine de caracteres
 * Résultat     : true si un alignement >= 5 est détecté,
 *                false sinon
 * Variables locales :
 *    directions[4][2] : tableau des 4 directions à tester
 *    compteur         : entier comptant le nombre de pions
 *    dx, dy, x, y     : entiers pour naviguer sur le plateau
 * -------------------------------------------------------
 */
bool verifier_alignement(const Plateau *p, int ligne, int colonne, char symbole) {
    // 4 directions de base
    int directions[4][2] = {
        {0, 1},   // horizontal
        {1, 0},   // vertical
        {1, 1},   // diagonale "\"
        {1, -1}   // diagonale "/"
    };

    // Pour chaque direction, on compte
    // le pion actuel + pions identiques dans les 2 sens
    for (int d = 0; d < 4; d++) {
        // compteur = 1 (le pion posé)
        int compteur = 1;

        // sens positif
        int dx = directions[d][0];
        int dy = directions[d][1];
        int x = ligne + dx;
        int y = colonne + dy;

        while (position_valide(x, y) &&
               p->plateau[x][y].symbole == symbole) {
            compteur++;
            x += dx;
            y += dy;
        }

        // sens négatif
        dx = -dx;
        dy = -dy;
        x = ligne + dx;
        y = colonne + dy;

        while (position_valide(x, y) &&
               p->plateau[x][y].symbole == symbole) {
            compteur++;
            x += dx;
            y += dy;
        }

        // Vérifier si on a 5 ou plus
        if (compteur >= 5) {
            return true;
        }
    }

    return false;
}


/*
 * -------------------------------------------------------
 * action : verifier_prise
 * -------------------------------------------------------
 * But          : Vérifier si, en posant un pion
 *                (ligne,colonne), on capture deux pions
 *                adverses (motif (joueur)(adv)(adv)(joueur)).
 *                Si oui, on enlève les 2 pions adverses
 *                et on incrémente le compteur de prises
 *                du joueur.
 * Données resultats: p: pointeur vers Plateau,
 * données ligne: entier,
 *                colonne:entier,
 *                symbole:chaine de caractère représentant le joueur ('O' ou 'X')
 * Variables locales :
 *    adversaire       : chaine de caractere ('O' si symbole == 'X', sinon 'X')
 *    directions[8][2] : tableau des 8 directions à tester
 *    x1,y1,x2,y2,x3,y3 : entier (indices pour repérer les pions)
 * -------------------------------------------------------
 */
void verifier_prise(Plateau *p, int ligne, int colonne, char symbole) {
    char adversaire = (symbole == 'X') ? 'O' : 'X';

    // 8 directions (haut, bas, gauche, droite, 4 diagonales)
    int directions[8][2] = {
        {1, 0},   // bas
        {-1, 0},  // haut
        {0, 1},   // droite
        {0, -1},  // gauche
        {1, 1},   // diag bas-droite
        {1, -1},  // diag bas-gauche
        {-1, 1},  // diag haut-droite
        {-1, -1}  // diag haut-gauche
    };

    // Vérifier le pattern (pion)(adversaire)(adversaire)(pion)
    for (int i = 0; i < 8; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int x1 = ligne + dx;
        int y1 = colonne + dy;
        int x2 = x1 + dx;
        int y2 = y1 + dy;
        int x3 = x2 + dx;
        int y3 = y2 + dy;

        // On n'utilise pas continue, on englobe la suite dans un if
        if (position_valide(x1, y1) &&
            position_valide(x2, y2) &&
            position_valide(x3, y3))
        {
            // Pattern exact : (symbole, adversaire, adversaire, symbole)
            if (p->plateau[x1][y1].symbole == adversaire &&
                p->plateau[x2][y2].symbole == adversaire &&
                p->plateau[x3][y3].symbole == symbole) {

                // On supprime les 2 pions adverses
                p->plateau[x1][y1].symbole = '.';
                p->plateau[x2][y2].symbole = '.';

                // Incrémenter les prises
                if (symbole == 'X') {
                    p->prises_joueur_X += 2;
                } else {
                    p->prises_joueur_O += 2;
                }
            }
        }
    }
}


/**
 * -------------------------------------------------------
 *  Fonction : tour_de_jeu
 * -------------------------------------------------------
 *  Rôle :
 *    - Demander au joueur en cours de saisir un coup
 *    - Gérer l'abandon si (0,0)
 *    - Placer le pion si possible
 *    - Vérifier les prises et l’alignement
 *  donnés :
 *    - plateau : pointeur vers la structure du plateau
 *    - joueur  : chaine de caractère représentant le joueur ('O' ou 'X')
 *  Variables Locales :
 *    - ligne,colonne: entier représentant les lignes et les colennes du plateau
 *    - ch: entier pour vider le buffer
 *  resultat :
 *    -  0 si le coup est normal et la partie continue
 *    -  1 si le joueur a gagné (par alignement ou par 10 prises)
 *    - -1 si le joueur abandonne
 * -------------------------------------------------------
 */
int tour_de_jeu(Plateau *plateau, char joueur) {
    // Affiche l’état actuel (on peut le faire ici ou dans main)
    afficher_plateau(plateau);

    // Demander le coup
    printf("Joueur %c, entrez votre coup (ligne,colonne) [0,0 pour abandon] : ", joueur);
    int ligne, colonne;

    // Vérifier la validité du format
    if (scanf("%d,%d", &ligne, &colonne) != 2) {
        // Mauvais format => on ignore ce tour et on retourne 0 (continuer)
        printf("Format invalide! (ex: 5,10)\n");
        // Vider le buffer
        // si on ne vide pas le buffer dans le cas d'une mauvaise saisie 5,99 et d'un correction par la suite en 5,9 le code prendra 5,5
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        return 0;
    }

    // Abandon
    if (ligne == 0 && colonne == 0) {
        return -1; // signal d'abandon
    }

    // Conversion indices (1..19) -> (0..18)
    ligne   -= 1;
    colonne -= 1;

    // Placer le pion si possible
    if (!placer_pion(plateau, ligne, colonne, joueur)) {
        printf("Coup invalide (hors-limites ou case non vide). Réessayez.\n");
        return 0;
    }

    // Vérifier si on capture des pions
    verifier_prise(plateau, ligne, colonne, joueur);

    // Vérifier alignement >= 5
    if (verifier_alignement(plateau, ligne, colonne, joueur)) {
        return 1; // alignement => victoire
    }

    // Vérifier victoire par 10 prises
    if (plateau->prises_joueur_X >= 10) {
        if (joueur == 'X') {
            return 1; // X gagne
        }
    }
    if (plateau->prises_joueur_O >= 10) {
        if (joueur == 'O') {
            return 1; // O gagne
        }
    }

    return 0; // pas de victoire ni d'abandon => la partie continue
}


/**
 * -------------------------------------------------------
 *  Fonction principale : main
 * -------------------------------------------------------
 *  Rôle :
 *    - Créer et initialiser le plateau
 *    - Déterminer qui commence ('O')
 *    - Gérer la boucle de jeu (appels successifs à tour_de_jeu)
 *    - Annoncer le vainqueur ou l'abandon
 * -------------------------------------------------------
 */
int main() {
    Plateau plateau;
    initialiser_plateau(&plateau);

    // On choisit le joueur qui débute
    char joueur = 'O';

    bool partieFinie = false;

    // Boucle de jeu
    while (!partieFinie) {
        int resultat = tour_de_jeu(&plateau, joueur);

        if (resultat == -1) {
            // Abandon
            printf("Joueur %c abandonne! ", joueur);
            printf("Le joueur %c est donc vainqueur!\n", (joueur == 'X') ? 'O' : 'X');
            partieFinie = true;
        }
        else if (resultat == 1) {
            // Le joueur courant gagne
            afficher_plateau(&plateau);
            printf("Joueur %c gagne la partie!\n", joueur);
            partieFinie = true;
        }
        else {
            // --- AJOUT pour match nul ---
            // Si personne n'a gagné et que le plateau est plein => match nul
            if (est_plein(&plateau)) {
                afficher_plateau(&plateau);
                printf("Match nul! Le plateau est plein et aucun joueur n'a pu remplir les conditions de victoire.\n");
                partieFinie = true;
            }
            else {
                // Sinon, on continue le jeu en changeant de joueur
                joueur = (joueur == 'X') ? 'O' : 'X';
            }
        }
    }
}