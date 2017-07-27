main:
	gcc -o a.exe ./utils/*?.c main.c

run:
	chcp 65001
	./a.exe .\\utils\\data

clean:
	rm a.exe
