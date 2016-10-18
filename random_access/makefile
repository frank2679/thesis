random: main.o module.o my_math.o
	gcc main.o module.o my_math.o -lm -o random_access 
main.o: main.c 
	gcc main.c -c 
module.o: module.c 
	gcc module.c -c 
my_math.o: my_math.c	
	gcc my_math.c -c 
clean: 
	rm -rf *.o
run: 
	./random_access
