all:
	gcc -fPIC -c lib1.c -o d1.o 
	gcc -fPIC -c lib2.c -o d2.o

	gcc -shared -o libd1.so d1.o -lm
	gcc -shared -o libd2.so d2.o -lm

	gcc prog2.c -L. -ldl -o main2.out -Wl,-rpath -Wl,.
	gcc prog1.c -L. -ld1 -o main1.out -Wl,-rpath -Wl,.