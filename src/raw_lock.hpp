#pragma once

#include <termios.h>

namespace nsn {
	struct [[nodiscard]] raw_lock {
		const ::termios cooked;

		raw_lock();

		~raw_lock();
	};
}
