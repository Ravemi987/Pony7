# ==========================================
# Makefile Général pour Projets C
# ==========================================

# Nom de l'exécutable final
TARGET = main

# Compilateur
CC = gcc

# Options de compilation (Warnings pour éviter les erreurs bêtes + Debug)
CFLAGS = -Wall -Wextra -g -O2

# Bibliothèques (ex: -lm pour la bibliothèque mathématique si besoin)
LIBS = -lm

# Détection automatique de tous les fichiers .c
SRCS = $(wildcard *.c)

# Transformation des noms .c en .o
OBJS = $(SRCS:.c=.o)

# Règle par défaut : compiler le projet
all: $(TARGET)

# Liaison des fichiers objets pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	@echo "------------------------------------------------"
	@echo "Compilation terminée : ./$(TARGET) est prêt."
	@echo "------------------------------------------------"

# Compilation de chaque fichier .c en fichier .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers temporaires
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Nettoyage effectué."

# Recompiler de zéro
re: clean all

.PHONY: all clean re