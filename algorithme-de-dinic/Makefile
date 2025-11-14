# Makefile pour l'implémentation de Dinic

# Compilateur et options
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2

# Nom de l'exécutable
TARGET = dinic

# Sources et headers
SRCS = dinic.c
HDRS = dinic.h

# Objets générés
OBJS = $(SRCS:.c=.o)

# --------------------------------------------------
# Cibles factices
# .PHONY : ces cibles ne correspondent pas à un fichier
# run    : compile puis exécute le programme
# all     : cible par défaut
# clean   : suppression des fichiers générés
# --------------------------------------------------
.PHONY: all run clean

#---------------------------------------------------
# all : compile l'exécutable
#---------------------------------------------------
all: $(TARGET)

#---------------------------------------------------
# run : compile si nécessaire, puis exécute
# UTILISATION : make run FILE=<chemin_du_fichier_dimacs>
# génère resultat.txt dans le répertoire courant
#---------------------------------------------------
run: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
	  echo "Erreur : veuillez spécifier FILE=<fichier DIMACS>"; exit 1; \
	fi
	@echo "Exécution de '$(TARGET)' sur $(FILE)'"
	@./$(TARGET) $(FILE)
	@echo "-> resultat.txt créé"

#---------------------------------------------------
# Règle de création de l'exécutable
# $@ = nom de la cible
# $^ = liste des dépendances
#---------------------------------------------------
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

#---------------------------------------------------
# Règle générique : .c -> .o
# $< = première dépendance (.c)
#---------------------------------------------------
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

#---------------------------------------------------
# clean : supprime exécutables, objets, et resultat.txt
#---------------------------------------------------
clean:
	rm -f $(TARGET) $(OBJS) resultat.txt
