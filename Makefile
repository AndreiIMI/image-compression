build:
	gcc -std=gnu11 -g -Wall -o quadtree quadtree.c func.c
run:
	./quadtree
clean:
	rm quadtree
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./quadtree