# Dinic – Algorithme de flot maximum

Ce projet implémente en C l’algorithme de **Dinic** pour le calcul du flot maximum dans un réseau orienté, à partir d’un format d’entrée DIMACS.

---

## 📁 Structure du projet

```
├── dinic.h        # Déclarations des structures et prototypes
├── dinic.c        # Implémentation de Dinic (buildGraph, BFS, augmentation, etc.)
├── Makefile       # Script de compilation et d’exécution
└── README.md      # Documentation et instructions
```

---

## 🛠 Prérequis

* **GCC** (ou tout compilateur compatible C11)
* **Make**

Vérifiez la présence de `gcc` et `make` :

```bash
gcc --version
make --version
```

---

## Compilation

Dans le répertoire racine du projet, exécutez :

```bash
make
```

* `ou` compilera les sources et générera :

  * L’exécutable `dinic`
  * Les fichiers objets `dinic.o`

---

## Exécution

### 1. Avec la cible `run`

Le **Makefile** fournit une cible \`\` pour compiler et exécuter en une seule commande :

```bash
make run FILE=<chemin_vers_fichier.dimacs>
```

* \`\` : chemin vers votre fichier DIMACS (ex. `R1`).
* Le programme produit ensuite \`\` dans le répertoire courant.
* Exemple :

  ```bash
  make run FILE=R1
  # Exécution : dinic R1
  # -> resultat.txt créé
  ```

### 2. Exécution manuelle

Si vous préférez :

```bash
./dinic <chemin_vers_fichier.dimacs>
```

Cela génère également \`\`.

---

## 📝 Format d’entrée (DIMACS)

Le fichier DIMACS doit respecter la syntaxe suivante :

```
c Ceci est un commentaire
p [max] <nb_sommets> <nb_arcs>
n <id_sommet> s    # définit la source
n <id_sommet> t    # définit le puits
a <u> <v> <capacité>
```

* \`\` : lignes de commentaire.
* `ou` : `n` = nombre de sommets, `m` = nombre d'arcs.
* `, ` : identifie la source et le puits.
* \`\` : arc de `u` vers `v` de capacité `c`.

---

## 📄 Format de sortie (`resultat.txt`)

Le programme écrit dans \`\` :

```
Flot maximal : <valeur_du_flot>

Flux sur les arcs :
<u1> -> <v1> : flux <f1> / capacité <c1>
<u2> -> <v2> : flux <f2> / capacité <c2>
...
```

* \`\` : valeur entière du flot de la source vers le puits.
* \`\` : pour chaque arc du réseau initial.

---

## Nettoyage

Pour supprimer l’exécutable, les objets et le fichier de résultat :

```bash
make clean
```

---
## 📬 Contact

Pour questions, suggestions ou rapports de bugs, veuillez contacter Amine.Nakrou@polytech-lille.net .
