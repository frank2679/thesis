random: main.o module.o
	gcc -g main.o module.o -lm -o simu_random-access 
main.o: main.c 
	gcc main.c -c 
module.o: module.c 
	gcc module.c -c 
clean: 
	rm -rf *.o
run: 
	./simu_random-access
