/*** includes ***/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f)

#define TERM_FLAGS_I 		~(BRKINT | ICRNL | INPCK | ISTRIP | IXON)
#define TERM_FLAGS_O		~(OPOST)
#define TERM_FLAGS_C 		(CS8)
#define TERM_FLAGS_L		~(ECHO | ICANON | IEXTEN | ISIG)

/*** data ***/

struct termios orig_termios;

/*** terminal ***/

void die(const char *s) {
  	perror(s);
  	exit(1);
}

void disableRawMode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("tcsetattr");
}

void enableRawMode() {
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
		die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= TERM_FLAGS_I;
	raw.c_oflag &= TERM_FLAGS_O;
	raw.c_cflag |= TERM_FLAGS_C;
	raw.c_lflag &= TERM_FLAGS_L;
	raw.c_cc[VMIN] = 0;
  	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

/*** init ***/

int main() {
	enableRawMode();

	while (1) {
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("read");
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == CTRL_KEY('q')) break;
	}

	return 0;
}