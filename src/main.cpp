#include "keys.hpp"
#include "term.hpp"

#include <print>

int main() {
	nsn::term term;

	term.screen_alternate(true);
	term.cursor_alternate(true);
	term.cursor_invisible(true);

	term.echo(false);
	term.signal(false);

	while (true) {
		if (nsn::keys key = term.read_key(); key == nsn::keys::ctrl_q) {
			break;
		} else if (char c = static_cast<char>(key)) {
			std::print("{}\v\r", c);
		}
	}
}
