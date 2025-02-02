#include "read_file.hpp"

#include <cstddef>
#include <cstdio>
#include <string>

std::string nsn::read_file(std::FILE* stream) {
	std::string result;
	std::size_t chunk = 1024;
	while (stream) {
		std::string buffer = std::string(chunk, '\0');
		const std::size_t bytes = std::fread(buffer.data(), 1, chunk, stream);
		result += buffer.substr(0, bytes);
		if (bytes != chunk) {
			break;
		}
		chunk *= 2;
	}
	return result;
}
