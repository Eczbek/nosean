#include "config.hpp"

#include <chrono>
#include <cstdio>
#include <print>
#include <string>
#include <thread>

#include <xieite/io/file.hpp>
#include <xieite/io/keys.hpp>
#include <xieite/io/keys_ch.hpp>
#include <xieite/io/read.hpp>
#include <xieite/io/term.hpp>
#include <xieite/sys/exit.hpp>

using namespace std::literals;

int main(int argc, char** argv) {
	if (argc > 2) {
		std::puts("Usage: `nsn [file]`");
		return xieite::exit::err_usage;
	} else if (argc == 2) {
		auto file = xieite::file(argv[1], "r");
		if (!file) {
			std::print("Failed to open file '{}'\n", argv[1]);
			return xieite::exit::err_no_input;
		}
		nsn::file_buf = xieite::read(file.get());
	}

	xieite::term term;
	term.screen_alt(true);
	term.cursor_alt(true);
	term.cursor_invis(true);
	term.echo(false);
	term.signal(false);
	term.block(false);

	while (nsn::running) {
		std::this_thread::sleep_for(10ms);

		// Draw border
		const auto [height, width] = term.screen_size();
		nsn::screen_height = height;
		nsn::screen_width = width;
		term.clear_screen();
		term.set_cursor(0, 0);
		std::fputs(std::string(width, '#').c_str(), term.out);
		for (int y = 1; y < (height - 1); ++y) {
			std::fputs("\v\r#", term.out);
			term.set_cursor(y, width - 1);
			std::fputc('#', term.out);
		}
		std::fputs("\v\r", term.out);
		std::fputs(std::string(width, '#').c_str(), term.out);
		std::fflush(term.out);

		// Read input
		xieite::keys key = term.read_key();
		if (const auto action = nsn::actions.find(key); action != nsn::actions.end()) {
			action->second();
		} else {
			switch (key) {
			case xieite::keys::backsp:
				// TODO: Erase character
				break;
			default:
				if (const char c = xieite::keys_ch(key)) {
					// TODO: Insert character
				}
			}
		}
	}

	term.cursor_invis(false);
	term.cursor_alt(false);
	term.screen_alt(false);
}
