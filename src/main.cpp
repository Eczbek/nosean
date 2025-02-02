#include <cstdio>
#include <print>
#include "raw_lock.hpp"

int main() {
	nsn::raw_lock raw_lock;

	while (true) {
		const int input = std::getchar();
		if (input == EOF) {
			break;
		}
		const char c = static_cast<char>(input);
		std::print("{}\v\r", c);
		if (c == 'Q') {
			break;
		}
	}
}
