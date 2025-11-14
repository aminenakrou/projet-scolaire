CC = gcc
CFLAGS = -Wall -Wextra -O2

# Cibles des exécutables principaux et de test
all: pent test_case1 test_case2

pent: projet.c
	$(CC) $(CFLAGS) -o pent projet.c

test_case1: projet.c
	$(CC) $(CFLAGS) -DTEST=1 -o test_case1 projet.c

test_case2: projet.c
	$(CC) $(CFLAGS) -DTEST=2 -o test_case2 projet.c

clean:
	rm -f pent test_case1 test_case2 *.o

.PHONY: all clean
