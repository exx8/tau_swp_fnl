FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm
all: cluster
clean:
	rm -rf *.o cluster



cluster: cluster.o
	gcc cluster.o -o cluster $(LIBS)
cluster.o: cluster.c
	gcc $(FLAGS) -c cluster.c