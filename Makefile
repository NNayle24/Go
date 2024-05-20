# Nom de l'exécutable final
TARGET = classiGo

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -I. -pthread

# Liste des fichiers source
SRCS = board.c item.c list.c table.c

# Génère la liste des fichiers objets correspondants
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour compiler les fichiers source en objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
