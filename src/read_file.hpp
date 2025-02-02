#pragma once

#include <cstdio>
#include <string>

namespace nsn {
	[[nodiscard]] std::string read_file(std::FILE* stream);
}
