#include "read_file.hpp"
#include "term.hpp"

#include <cmath>
#include <cstdio>
#include <print>
#include <string>
#include <utility>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

nsn::term::term(std::FILE* in, std::FILE* out)
: in(in)
, out(out)
, blocking(::fcntl(::fileno(in), F_GETFL))
, cooked([this] {
	::termios cooked;
	::tcgetattr(::fileno(this->in), &cooked);
	return cooked;
}())
, is_block(this->blocking & ~O_NONBLOCK)
, is_echo(this->cooked.c_lflag & ECHO)
, is_canon(this->cooked.c_lflag & ICANON)
, is_signal(
	(this->cooked.c_iflag & IXON)
	|| (this->cooked.c_iflag & ICRNL)
	|| (this->cooked.c_lflag & IEXTEN)
	|| (this->cooked.c_lflag & ISIG)
)
, is_proc(this->cooked.c_oflag & OPOST)
{ this->flush(); }

nsn::term::~term() {
	this->reset_mode();
	this->reset_style();
	std::fflush(this->out);
}

void nsn::term::block(bool x) {
	if (this->is_block != x) {
		this->is_block = x;
		this->flush();
	}
}

void nsn::term::echo(bool x) {
	if (this->is_echo != x) {
		this->is_echo = x;
		this->flush();
	}
}

void nsn::term::canon(bool x) {
	if (this->is_canon != x) {
		this->is_canon = x;
		this->flush();
	}
}

void nsn::term::signal(bool x) {
	if (this->is_signal != x) {
		this->is_signal = x;
		this->flush();
	}
}

void nsn::term::proc(bool x) {
	if (this->is_proc != x) {
		this->is_proc = x;
		this->flush();
	}
}

void nsn::term::fg(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
	std::print(this->out, "\x1B[38;2;{};{};{}m", r, g, b);
}

void nsn::term::fg_reset() {
	std::print(this->out, "\x1B[38m");
}

void nsn::term::bg(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
	std::print(this->out, "\x1B[48;2;{};{};{}m", r, g, b);
}

void nsn::term::bg_reset() {
	std::print(this->out, "\x1B[48m");
}

void nsn::term::bold(bool x) {
	std::print(this->out, "{}", x ? "\x1B[1m" : "\x1B[21m");
}

void nsn::term::italic(bool x) {
	std::print(this->out, "{}", x ? "\x1B[3m" : "\x1B[23m");
}

void nsn::term::underline(bool x) {
	std::print(this->out, "{}", x ? "\x1B[4m" : "\x1B[24m");
}

void nsn::term::blink(bool x) {
	std::print(this->out, "{}", x ? "\x1B[5m" : "\x1B[25m");
}

void nsn::term::invisible(bool x) {
	std::print(this->out, "{}", x ? "\x1B[8m" : "\x1B[28m");
}

void nsn::term::strike(bool x) {
	std::print(this->out, "{}", x ? "\x1B[9m" : "\x1B[29m");
}

void nsn::term::invert(bool x) {
	std::print(this->out, "{}", x ? "\x1B[7m" : "\x1B[27m");
}

void nsn::term::reset_style() {
	this->screen_alternate(true);
	this->clear_screen();
	this->screen_alternate(false);

	this->cursor_alternate(true);
	this->set_cursor(0, 0);
	this->cursor_alternate(false);

	this->cursor_invisible(false);

	std::print(this->out, "\x1B[0m");
}

void nsn::term::reset_mode() {
	::fcntl(::fileno(this->in), F_SETFL, this->blocking);
	::tcsetattr(::fileno(this->in), TCSANOW, &this->cooked);
}

std::pair<int, int> nsn::term::get_cursor() {
	const bool prev_canon = this->is_canon;
	this->canon(false);
	std::print(this->out, "\x1B[6n");
	int x;
	int y;
	std::fscanf(this->in, "\x1B[%i;%iR", &x, &y);
	this->canon(prev_canon);
	return std::make_pair(x - 1, y - 1);
}

void nsn::term::set_cursor(int x, int y) {
	std::print(this->out, "\x1B[{};{}H", x, y);
}

void nsn::term::move_cursor(int x, int y) {
	std::print(this->out, "\x1B[{}{}\x1B[{}{}", std::abs(x), "CD"[x < 0], std::abs(y), "BA"[y < 0]);
}

void nsn::term::cursor_invisible(bool x) {
	std::print(this->out, "\x1B[?25{}", "hl"[x]);
}

void nsn::term::cursor_block(bool blink) {
	std::print(this->out, "\1\x1B[{}q\2", 2 - blink);
}

void nsn::term::cursor_underscore(bool blink) {
	std::print(this->out, "\1\x1B[{}q\2", 4 - blink);
}

void nsn::term::cursor_pipe(bool blink) {
	std::print(this->out, "\1\x1B[{}q\2", 6 - blink);
}

void nsn::term::cursor_alternate(bool x) {
	std::print(this->out, "\x1B[{}", "us"[x]);
}

void nsn::term::screen_alternate(bool x) {
	std::print(this->out, "\x1B[?47{}", "lh"[x]);
}

std::pair<int, int> nsn::term::screen_size() {
	::winsize size;
	::ioctl(::fileno(this->in), TIOCGWINSZ, &size);
	return std::make_pair(size.ws_col, size.ws_row);
}

void nsn::term::clear_screen() {
	std::print(this->out, "\x1B[2J");
}

void nsn::term::clear_screen_until() {
	std::print(this->out, "\x1B[1J");
}

void nsn::term::clear_screen_from() {
	std::print(this->out, "\x1B[0J");
}

void nsn::term::clear_line() {
	std::print(this->out, "\x1B[2K");
}

void nsn::term::clear_line_until() {
	std::print(this->out, "\x1B[1K");
}

void nsn::term::clear_line_from() {
	std::print(this->out, "\x1B[0K");
}

int nsn::term::read_char() {
	const bool prev_canon = this->is_canon;
	this->canon(false);
	const int c = std::fgetc(this->in);
	this->canon(prev_canon);
	return c;
}

[[nodiscard]] std::string nsn::term::read_string() {
	const bool prev_block = this->is_block;
	const bool prev_canon = this->is_canon;
	this->block(false);
	this->canon(false);
	const std::string input = nsn::read_file(this->in);
	this->block(prev_block);
	this->canon(prev_canon);
	return input;
}

nsn::keys nsn::term::read_key() {
	struct guard {
		bool& is_block;
		bool prev_block = this->is_block;

		~guard() {
			this->is_block = this->prev_block;
		}
	};
	const guard _ = guard(this->is_block);

	const int c0 = this->read_char();
	this->block(false);
	switch (c0) {
	case 0x00:
		switch (const int c1 = this->read_char()) {
		case 0x00:
			return nsn::keys::pause;
		case 0x03:
			return nsn::keys::null;
		default:
			std::ungetc(c1, this->in);
		}
		break;
	case 0x01:
		return nsn::keys::ctrl_a;
	case 0x02:
		return nsn::keys::ctrl_b;
	case 0x03:
		return nsn::keys::ctrl_c;
	case 0x04:
		return nsn::keys::ctrl_d;
	case 0x05:
		return nsn::keys::ctrl_e;
	case 0x06:
		return nsn::keys::ctrl_f;
	case 0x07:
		return nsn::keys::ctrl_g;
	case 0x08:
		return nsn::keys::ctrl_h;
	case 0x09:
		return nsn::keys::ctrl_i;
	case 0x0A:
		return nsn::keys::ctrl_j;
	case 0x0B:
		return nsn::keys::ctrl_k;
	case 0x0C:
		return nsn::keys::ctrl_l;
	case 0x0D:
		return nsn::keys::ctrl_m;
	case 0x0E:
		return nsn::keys::ctrl_n;
	case 0x0F:
		return nsn::keys::ctrl_o;
	case 0x10:
		return nsn::keys::ctrl_p;
	case 0x11:
		return nsn::keys::ctrl_q;
	case 0x12:
		return nsn::keys::ctrl_r;
	case 0x13:
		return nsn::keys::ctrl_s;
	case 0x14:
		return nsn::keys::ctrl_t;
	case 0x15:
		return nsn::keys::ctrl_u;
	case 0x16:
		return nsn::keys::ctrl_v;
	case 0x17:
		return nsn::keys::ctrl_w;
	case 0x18:
		return nsn::keys::ctrl_x;
	case 0x19:
		return nsn::keys::ctrl_y;
	case 0x1A:
		return nsn::keys::ctrl_z;
	case 0x1B:
		switch (const int c1 = this->read_char()) {
		case 0x01:
			return nsn::keys::ctrl_alt_a;
		case 0x02:
			return nsn::keys::ctrl_alt_b;
		case 0x03:
			return nsn::keys::ctrl_alt_c;
		case 0x04:
			return nsn::keys::ctrl_alt_d;
		case 0x05:
			return nsn::keys::ctrl_alt_e;
		case 0x06:
			return nsn::keys::ctrl_alt_f;
		case 0x07:
			return nsn::keys::ctrl_alt_g;
		case 0x08:
			return nsn::keys::ctrl_alt_h;
		case 0x09:
			return nsn::keys::ctrl_alt_i;
		case 0x0A:
			return nsn::keys::ctrl_alt_j;
		case 0x0B:
			return nsn::keys::ctrl_alt_k;
		case 0x0C:
			return nsn::keys::ctrl_alt_l;
		case 0x0D:
			return nsn::keys::ctrl_alt_m;
		case 0x0E:
			return nsn::keys::ctrl_alt_n;
		case 0x0F:
			return nsn::keys::ctrl_alt_o;
		case 0x10:
			return nsn::keys::ctrl_alt_p;
		case 0x11:
			return nsn::keys::ctrl_alt_q;
		case 0x12:
			return nsn::keys::ctrl_alt_r;
		case 0x13:
			return nsn::keys::ctrl_alt_s;
		case 0x14:
			return nsn::keys::ctrl_alt_t;
		case 0x15:
			return nsn::keys::ctrl_alt_u;
		case 0x16:
			return nsn::keys::ctrl_alt_v;
		case 0x17:
			return nsn::keys::ctrl_alt_w;
		case 0x18:
			return nsn::keys::ctrl_alt_x;
		case 0x19:
			return nsn::keys::ctrl_alt_y;
		case 0x1A:
			return nsn::keys::ctrl_alt_z;
		case 0x20:
			return nsn::keys::alt_space;
		case 0x21:
			return nsn::keys::alt_exclamation_mark;
		case 0x22:
			return nsn::keys::alt_quotes;
		case 0x23:
			return nsn::keys::alt_hashtag;
		case 0x24:
			return nsn::keys::alt_dollar;
		case 0x25:
			return nsn::keys::alt_percent;
		case 0x26:
			return nsn::keys::alt_ampersand;
		case 0x27:
			return nsn::keys::alt_apostrophe;
		case 0x28:
			return nsn::keys::alt_left_parenthesis;
		case 0x29:
			return nsn::keys::alt_right_parenthesis;
		case 0x2A:
			return nsn::keys::alt_star;
		case 0x2B:
			return nsn::keys::alt_plus;
		case 0x2C:
			return nsn::keys::alt_comma;
		case 0x2D:
			return nsn::keys::alt_dash;
		case 0x2E:
			return nsn::keys::alt_dot;
		case 0x2F:
			return nsn::keys::alt_slash;
		case 0x30:
			return nsn::keys::alt_zero;
		case 0x31:
			return nsn::keys::alt_one;
		case 0x32:
			return nsn::keys::alt_two;
		case 0x33:
			return nsn::keys::alt_three;
		case 0x34:
			return nsn::keys::alt_four;
		case 0x35:
			return nsn::keys::alt_five;
		case 0x36:
			return nsn::keys::alt_six;
		case 0x37:
			return nsn::keys::alt_seven;
		case 0x38:
			return nsn::keys::alt_eight;
		case 0x39:
			return nsn::keys::alt_nine;
		case 0x3A:
			return nsn::keys::alt_colon;
		case 0x3B:
			return nsn::keys::alt_semicolon;
		case 0x3C:
			return nsn::keys::alt_less_than;
		case 0x3D:
			return nsn::keys::alt_equal;
		case 0x3E:
			return nsn::keys::alt_greater_than;
		case 0x3F:
			return nsn::keys::alt_question_mark;
		case 0x40:
			return nsn::keys::alt_strudel;
		case 0x41:
			return nsn::keys::alt_A;
		case 0x42:
			return nsn::keys::alt_B;
		case 0x43:
			return nsn::keys::alt_C;
		case 0x44:
			return nsn::keys::alt_D;
		case 0x45:
			return nsn::keys::alt_E;
		case 0x46:
			return nsn::keys::alt_F;
		case 0x47:
			return nsn::keys::alt_G;
		case 0x48:
			return nsn::keys::alt_H;
		case 0x49:
			return nsn::keys::alt_I;
		case 0x4A:
			return nsn::keys::alt_J;
		case 0x4B:
			return nsn::keys::alt_K;
		case 0x4C:
			return nsn::keys::alt_L;
		case 0x4D:
			return nsn::keys::alt_M;
		case 0x4E:
			return nsn::keys::alt_N;
		case 0x4F:
			switch (const int c2 = this->read_char()) {
			case 0x50:
				return nsn::keys::f1;
			case 0x51:
				return nsn::keys::f2;
			case 0x52:
				return nsn::keys::f3;
			case 0x53:
				return nsn::keys::f4;
			default:
				std::ungetc(c2, this->in);
			}
			return nsn::keys::alt_O;
		case 0x50:
			return nsn::keys::alt_P;
		case 0x51:
			return nsn::keys::alt_Q;
		case 0x52:
			return nsn::keys::alt_R;
		case 0x53:
			return nsn::keys::alt_S;
		case 0x54:
			return nsn::keys::alt_T;
		case 0x55:
			return nsn::keys::alt_U;
		case 0x56:
			return nsn::keys::alt_V;
		case 0x57:
			return nsn::keys::alt_W;
		case 0x58:
			return nsn::keys::alt_X;
		case 0x59:
			return nsn::keys::alt_Y;
		case 0x5A:
			return nsn::keys::alt_Z;
		case 0x5B:
			{
				const int c2 = this->read_char();
				const int c3 = this->read_char();
				if (c3 == 0x7E) {
					switch (c2) {
					case 0x20:
						return nsn::keys::numpad_zero;
					case 0x21:
						return nsn::keys::numpad_dot;
					case 0x23:
						return nsn::keys::numpad_nine;
					case 0x24:
						return nsn::keys::numpad_three;
					}
				}
				switch (c2) {
				case 0x29:
					return nsn::keys::numpad_eight;
				case 0x2A:
					return nsn::keys::numpad_two;
				case 0x2B:
					return nsn::keys::numpad_six;
				case 0x2C:
					return nsn::keys::numpad_four;
				case 0x2D:
					return nsn::keys::numpad_five;
				case 0x2E:
					return nsn::keys::numpad_one;
				case 0x30:
					return nsn::keys::numpad_seven;
				case 0x31:
					if (const int c4 = this->read_char(); c4 == 0x7E) {
						switch (c3) {
						case 0x35:
							return nsn::keys::f5;
						case 0x37:
							return nsn::keys::f6;
						case 0x38:
							return nsn::keys::f7;
						case 0x39:
							return nsn::keys::f8;
						}
						std::ungetc(c4, this->in);
					}
					break;
				case 0x32:
					if (const int c4 = this->read_char(); c4 == 0x7E) {
						switch (c3) {
						case 0x30:
							return nsn::keys::f9;
						case 0x31:
							return nsn::keys::f10;
						case 0x33:
							return nsn::keys::f11;
						case 0x34:
							return nsn::keys::f12;
						}
						std::ungetc(c4, this->in);
					}
					break;
				case 0x5B:
					switch (c3) {
					case 0x41:
						return nsn::keys::f1;
					case 0x42:
						return nsn::keys::f2;
					case 0x43:
						return nsn::keys::f3;
					case 0x44:
						return nsn::keys::f4;
					case 0x45:
						return nsn::keys::f5;
					}
					break;
				}
				std::ungetc(c3, this->in);
				std::ungetc(c2, this->in);
			}
			return nsn::keys::alt_left_bracket;
		case 0x5C:
			return nsn::keys::alt_backslash;
		case 0x5D:
			return nsn::keys::alt_right_bracket;
		case 0x5E:
			return nsn::keys::alt_caret;
		case 0x5F:
			return nsn::keys::alt_underscore;
		case 0x60:
			return nsn::keys::alt_grave;
		case 0x61:
			return nsn::keys::alt_a;
		case 0x62:
			return nsn::keys::alt_b;
		case 0x63:
			return nsn::keys::alt_c;
		case 0x64:
			return nsn::keys::alt_d;
		case 0x65:
			return nsn::keys::alt_e;
		case 0x66:
			return nsn::keys::alt_f;
		case 0x67:
			return nsn::keys::alt_g;
		case 0x68:
			return nsn::keys::alt_h;
		case 0x69:
			return nsn::keys::alt_i;
		case 0x6A:
			return nsn::keys::alt_j;
		case 0x6B:
			return nsn::keys::alt_k;
		case 0x6C:
			return nsn::keys::alt_l;
		case 0x6D:
			return nsn::keys::alt_m;
		case 0x6E:
			return nsn::keys::alt_n;
		case 0x6F:
			return nsn::keys::alt_o;
		case 0x70:
			return nsn::keys::alt_p;
		case 0x71:
			return nsn::keys::alt_q;
		case 0x72:
			return nsn::keys::alt_r;
		case 0x73:
			return nsn::keys::alt_s;
		case 0x74:
			return nsn::keys::alt_t;
		case 0x75:
			return nsn::keys::alt_u;
		case 0x76:
			return nsn::keys::alt_v;
		case 0x77:
			return nsn::keys::alt_w;
		case 0x78:
			return nsn::keys::alt_x;
		case 0x79:
			return nsn::keys::alt_y;
		case 0x7A:
			return nsn::keys::alt_z;
		case 0x7B:
			return nsn::keys::alt_left_brace;
		case 0x7C:
			return nsn::keys::alt_pipe;
		case 0x7D:
			return nsn::keys::alt_right_brace;
		case 0x7E:
			return nsn::keys::alt_tilde;
		case 0x7F:
			return nsn::keys::alt_backspace;
		default:
			std::ungetc(c1, this->in);
		}
		// TODO: Detect keys F13 through F24
		break;
	case 0x20:
		return nsn::keys::space;
	case 0x21:
		return nsn::keys::exclamation_mark;
	case 0x22:
		return nsn::keys::quotes;
	case 0x23:
		return nsn::keys::hashtag;
	case 0x24:
		return nsn::keys::dollar;
	case 0x25:
		return nsn::keys::percent;
	case 0x26:
		return nsn::keys::ampersand;
	case 0x27:
		return nsn::keys::apostrophe;
	case 0x28:
		return nsn::keys::left_parenthesis;
	case 0x29:
		return nsn::keys::right_parenthesis;
	case 0x2A:
		return nsn::keys::star;
	case 0x2B:
		return nsn::keys::plus;
	case 0x2C:
		return nsn::keys::comma;
	case 0x2D:
		return nsn::keys::dash;
	case 0x2E:
		return nsn::keys::dot;
	case 0x2F:
		return nsn::keys::slash;
	case 0x30:
		return nsn::keys::zero;
	case 0x31:
		return nsn::keys::one;
	case 0x32:
		return nsn::keys::two;
	case 0x33:
		return nsn::keys::three;
	case 0x34:
		return nsn::keys::four;
	case 0x35:
		return nsn::keys::five;
	case 0x36:
		return nsn::keys::six;
	case 0x37:
		return nsn::keys::seven;
	case 0x38:
		return nsn::keys::eight;
	case 0x39:
		return nsn::keys::nine;
	case 0x3A:
		return nsn::keys::colon;
	case 0x3B:
		return nsn::keys::semicolon;
	case 0x3C:
		return nsn::keys::less_than;
	case 0x3D:
		return nsn::keys::equal;
	case 0x3E:
		return nsn::keys::greater_than;
	case 0x3F:
		return nsn::keys::question_mark;
	case 0x40:
		return nsn::keys::strudel;
	case 0x41:
		return nsn::keys::A;
	case 0x42:
		return nsn::keys::B;
	case 0x43:
		return nsn::keys::C;
	case 0x44:
		return nsn::keys::D;
	case 0x45:
		return nsn::keys::E;
	case 0x46:
		return nsn::keys::F;
	case 0x47:
		return nsn::keys::G;
	case 0x48:
		return nsn::keys::H;
	case 0x49:
		return nsn::keys::I;
	case 0x4A:
		return nsn::keys::J;
	case 0x4B:
		return nsn::keys::K;
	case 0x4C:
		return nsn::keys::L;
	case 0x4D:
		return nsn::keys::M;
	case 0x4E:
		return nsn::keys::N;
	case 0x4F:
		return nsn::keys::O;
	case 0x50:
		return nsn::keys::P;
	case 0x51:
		return nsn::keys::Q;
	case 0x52:
		return nsn::keys::R;
	case 0x53:
		return nsn::keys::S;
	case 0x54:
		return nsn::keys::T;
	case 0x55:
		return nsn::keys::U;
	case 0x56:
		return nsn::keys::V;
	case 0x57:
		return nsn::keys::W;
	case 0x58:
		return nsn::keys::X;
	case 0x59:
		return nsn::keys::Y;
	case 0x5A:
		return nsn::keys::Z;
	case 0x5B:
		return nsn::keys::left_bracket;
	case 0x5C:
		return nsn::keys::backslash;
	case 0x5D:
		return nsn::keys::right_bracket;
	case 0x5E:
		return nsn::keys::caret;
	case 0x5F:
		return nsn::keys::underscore;
	case 0x60:
		return nsn::keys::grave;
	case 0x61:
		return nsn::keys::a;
	case 0x62:
		return nsn::keys::b;
	case 0x63:
		return nsn::keys::c;
	case 0x64:
		return nsn::keys::d;
	case 0x65:
		return nsn::keys::e;
	case 0x66:
		return nsn::keys::f;
	case 0x67:
		return nsn::keys::g;
	case 0x68:
		return nsn::keys::h;
	case 0x69:
		return nsn::keys::i;
	case 0x6A:
		return nsn::keys::j;
	case 0x6B:
		return nsn::keys::k;
	case 0x6C:
		return nsn::keys::l;
	case 0x6D:
		return nsn::keys::m;
	case 0x6E:
		return nsn::keys::n;
	case 0x6F:
		return nsn::keys::o;
	case 0x70:
		return nsn::keys::p;
	case 0x71:
		return nsn::keys::q;
	case 0x72:
		return nsn::keys::r;
	case 0x73:
		return nsn::keys::s;
	case 0x74:
		return nsn::keys::t;
	case 0x75:
		return nsn::keys::u;
	case 0x76:
		return nsn::keys::v;
	case 0x77:
		return nsn::keys::w;
	case 0x78:
		return nsn::keys::x;
	case 0x79:
		return nsn::keys::y;
	case 0x7A:
		return nsn::keys::z;
	case 0x7B:
		return nsn::keys::left_brace;
	case 0x7C:
		return nsn::keys::pipe;
	case 0x7D:
		return nsn::keys::right_brace;
	case 0x7E:
		return nsn::keys::tilde;
	case 0x7F:
		return nsn::keys::backspace;
	}
	std::ungetc(c0, this->in);
	return nsn::keys::unknown;
}

void nsn::term::flush() {
	::fcntl(::fileno(this->in), F_SETFL, this->blocking | (O_NONBLOCK * !this->is_block));
	::termios raw = this->cooked;
	raw.c_iflag &= ~static_cast<::tcflag_t>(
		(ICRNL * !this->is_signal)
		| (IXON * !this->is_signal)
	);
	raw.c_lflag &= ~static_cast<::tcflag_t>(
		(ECHO * !this->is_echo)
		| (ICANON * !this->is_canon)
		| (IEXTEN * !this->is_signal)
		| (ISIG * !this->is_signal)
	);
	raw.c_oflag &= ~static_cast<::tcflag_t>(OPOST * !this->is_proc);
	::tcsetattr(::fileno(this->in), TCSANOW, &raw);
}

