CFLAG = -Wall -Wextra -Werror
default:
	gcc -o main main.c chip8.c gui.c -lraylib

debug:
	gcc $(CFLAG) -g -o main main.c chip8.c gui.c -lraylib

clean:
	rm main
