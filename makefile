main:
	gcc -static -g -c src/*c 
	gcc -static -g -o deepnet *.o -lm -lc -l:libncurses.a 
	make clean
run:
	./deepnet
clean:
	rm *.o
