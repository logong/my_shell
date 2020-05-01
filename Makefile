demo1: demo1.o
demo1.o: demo1.c
	gcc -c demo1.c -o demo1.o
clean:
	-rm *.o demo1
