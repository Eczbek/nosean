#include "keys.hpp"
#include "read_file.hpp"
#include "term.hpp"
#include "config.hpp"

#include <chrono>
#include <cstdio>
#include <print>
#include <string>
#include <thread>

// Everything is broken
// i am well aware

using namespace std::chrono_literals;

int main(int argc, char** argv) {
	// load config
	nsn::load_config();

	std::string buf;
	int buf_y = 0;
	int buf_x = 0;

	if (argc > 2) {
		std::print("Usage: `nsn [file]`\n");
		return 64;
	} else if (argc == 2) {
		std::FILE* file = std::fopen(argv[1], "r");
		if (!file) {
			std::print("Failed to open file '{}'\n", argv[1]);
			return 66;
		}
		buf = nsn::read_file(file);
		std::fclose(file);
	}

	nsn::term term;
	term.screen_alternate(true);
	term.cursor_alternate(true);
	term.cursor_invisible(true);
	term.echo(false);
	term.signal(false);
	term.block(false);
	std::fflush(stdout);

	int prev_h = 0;
	int prev_w = 0;

	bool run = true;
	unsigned long long int tick = 0;
	while (tick++, run) {
		std::this_thread::sleep_for(10ms);

		term.set_cursor(0, 0);
		std::print("{}", tick);

		// Draw cursor
		term.set_cursor(buf_y, buf_x);
		term.bg(255, 0, 0);
		std::print(" ");

		// Read input
		nsn::keys key = term.read_key();

		if (key == nsn::keys::backspace)
			buf.pop_back();
		else if (key == nsn::get_keybind("quit"))
			run = false;
		else if (key == nsn::keys::s)
			++buf_y;
		else if (key == nsn::keys::w)
			--buf_y;
		else if (key == nsn::keys::d)
			++buf_x;
		else if (key == nsn::keys::a)
			--buf_x;
		else {
			if (char c = static_cast<char>(key)) {
				buf += c;
			}
		}
	}

	term.cursor_alternate(false);
	term.screen_alternate(false);
}
