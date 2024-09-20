make: phutball.c
	gcc -O3 -o phutball phutball.c -lm
	gcc -o phutballOriginal phutball.c -lm