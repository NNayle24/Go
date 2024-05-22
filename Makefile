# Nom de l'exécutable final
TARGET = my_program

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -I.

# Liste des fichiers source
SRCS = board.c item.c list.c table.c test_board.c board2.c

# Génère la liste des fichiers objets correspondants
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -pthread  -lm -o $@ $^

# Règle pour compiler les fichiers source en objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
