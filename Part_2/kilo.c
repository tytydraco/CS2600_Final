#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#define TERM_FLAGS_I 		~(BRKINT | ICRNL | INPCK | ISTRIP | IXON)
#define TERM_FLAGS_O		~(OPOST)
#define TERM_FLAGS_C 		(CS8)
#define TERM_FLAGS_L		~(ECHO | ICANON | IEXTEN | ISIG)

struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_iflag &= TERM_FLAGS_I;
	raw.c_oflag &= TERM_FLAGS_O;
	raw.c_cflag |= TERM_FLAGS_C;
	raw.c_lflag &= TERM_FLAGS_L;
	raw.c_cc[VMIN] = 0;
  	raw.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();

	while (1) {
		char c = '\0';
		read(STDIN_FILENO, &c, 1);
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == 'q') break;
	}

	return 0;
}