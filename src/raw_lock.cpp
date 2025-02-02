#include "raw_lock.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

nsn::raw_lock::raw_lock()
: cooked([] {
	::termios cooked;
	::tcgetattr(STDIN_FILENO, &cooked);
	return cooked;
}()) {
	::termios raw = this->cooked;
	raw.c_iflag &= ~(ICRNL | IXON);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_oflag &= ~(OPOST);
	::tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

nsn::raw_lock::~raw_lock() {
	::tcsetattr(STDIN_FILENO, TCSANOW, &this->cooked);
}
