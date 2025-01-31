#include <cstdio>
#include <print>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int main() {
	const int blocking_mode = ::fcntl(STDIN_FILENO, F_GETFL);
	const ::termios cooked_mode = ([] {
		::termios cooked_mode;
		::tcgetattr(STDIN_FILENO, &cooked_mode);
		return cooked_mode;
	})();
	bool input_blocking = !(blocking_mode & O_NONBLOCK);
	bool input_echoing = cooked_mode.c_lflag & ECHO;
	bool input_canonical = cooked_mode.c_lflag & ICANON;
	bool input_signals = (cooked_mode.c_iflag & IXON)
		|| (cooked_mode.c_iflag & ICRNL)
		|| (cooked_mode.c_lflag & IEXTEN)
		|| (cooked_mode.c_lflag & ISIG);
	bool output_processing = cooked_mode.c_oflag & OPOST;

	auto flush_mode = [&] {
		::fcntl(STDIN_FILENO, F_SETFL, blocking_mode | (O_NONBLOCK * !input_blocking));
		::termios raw_mode = cooked_mode;
		raw_mode.c_iflag &= ~static_cast<::tcflag_t>(
			(ICRNL * !input_signals)
			| (IXON * !input_signals)
		);
		raw_mode.c_lflag &= ~static_cast<::tcflag_t>(
			(ECHO * !input_echoing)
			| (ICANON * !input_canonical)
			| (IEXTEN * !input_signals)
			| (ISIG * !input_signals)
		);
		raw_mode.c_oflag &= ~static_cast<::tcflag_t>(OPOST * !output_processing);
		::tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);
	};

	const bool prev_echoing = input_echoing;
	const bool prev_canonical = input_canonical;
	input_echoing = false;
	input_canonical = false;
	flush_mode();
	while (true) {
		const int input = std::getchar();
		if (input == EOF) {
			break;
		}
		const char c = static_cast<char>(input);
		std::print("{}\n", c);
		if (c == 'Q') {
			break;
		}
	}
	input_echoing = prev_echoing;
	input_canonical = prev_canonical;
	flush_mode();

	::fcntl(STDIN_FILENO, F_SETFL, blocking_mode);
	::tcsetattr(STDIN_FILENO, TCSANOW, &cooked_mode);
}
