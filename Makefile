all: run

run: carros
	./$<

carros: carros.c
	gcc $< -o $@