default:
	gcc -o main main.c chip8.c -lraylib

debug:
	gcc -g -o main main.c chip8.c -lraylib

clean:
	rm main
