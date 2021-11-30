#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#define TERM_FLAGS_AND 		~(BRKINT | ICRNL | INPCK | ISTRIP | IXON | OPOST | ECHO | ICANON | IEXTEN | ISIG)
#define TERM_FLAGS_OR 		(CS8)

struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= TERM_FLAGS_AND;
	raw.c_lflag |= TERM_FLAGS_OR;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();

	char c;

	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
	}

	return 0;
}