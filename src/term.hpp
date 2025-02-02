#pragma once

#include "keys.hpp"

#include <cstdint>
#include <cstdio>
#include <string>
#include <utility>

#include <termios.h>

namespace nsn {
	struct term {
	public:
		std::FILE* in;
		std::FILE* out;

		[[nodiscard]] term(std::FILE* in = stdin, std::FILE* out = stdout);

		term(const nsn::term&) = delete;

		~term();

		void block(bool x);

		void echo(bool x);

		void canon(bool x);

		void signal(bool x);

		void proc(bool x);

		void fg(std::uint8_t r, std::uint8_t g, std::uint8_t b);

		void fg_reset();

		void bg(std::uint8_t r, std::uint8_t g, std::uint8_t b);

		void bg_reset();

		void bold(bool x);

		void italic(bool x);

		void underline(bool x);

		void blink(bool x);

		void invisible(bool x);

		void strike(bool x);
		
		void invert(bool x);

		void reset_style();
		
		void reset_mode();

		[[nodiscard]] std::pair<int, int> get_cursor();

		void set_cursor(int x, int y);

		void move_cursor(int x, int y);

		void cursor_invisible(bool x);

		void cursor_block(bool blink = false);

		void cursor_underscore(bool blink = false);

		void cursor_pipe(bool blink = false);

		void cursor_alternate(bool x);

		void screen_alternate(bool x);

		[[nodiscard]] std::pair<int, int> screen_size();

		void clear_screen();
		
		void clear_screen_until();
		
		void clear_screen_from();

		void clear_line();

		void clear_line_until();

		void clear_line_from();

		int read_char();

		std::string read_string();

		nsn::keys read_key();

	private:
		const int blocking;
		::termios cooked;

		bool is_block;
		bool is_echo;
		bool is_canon;
		bool is_signal;
		bool is_proc;

		void flush();
	};
}
