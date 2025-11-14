# Projet Pent – Polytech Lille ISIA

Ce dépôt contient l’implémentation du jeu de plateau Pent, réalisée dans le cadre de la spécialité ISIA (Informatique, Statistique et Intelligence Artificielle) à Polytech Lille.

## Présentation

Pent est un jeu de stratégie pour deux joueurs sur une grille 19x19. Le projet gère l’affichage du plateau, la saisie et validation des coups, ainsi que la détection des alignements et des captures de pions selon les règles officielles.

## Structure du projet

- `projet.c` : Code source principal du jeu Pent.
- `rapport.pdf` : Rapport détaillant les choix algorithmiques, la logique de programmation et les tests réalisés.
- `Makefile` : Automatisation de la compilation des différents exécutables correspondant aux cas de test ou au jeu principal.

## Fonctionnalités

- Initialisation, sauvegarde et affichage dynamique du plateau.
- Gestion alternée des coups et validation des saisies.
- Détection des victoires (alignements, captures) et gestion des fins de partie.
- Exécutables dédiés pour différents scénarios de test.
