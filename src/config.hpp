#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <xieite/io/keys.hpp>
#include <xieite/fn/tmp.hpp>

namespace nsn {
	inline const nlohmann::json config = nlohmann::json::parse(xieite::tmp(std::ifstream("../nsn_config.json")));

	inline std::string file_buf;

	inline std::size_t file_pos = 0;

	inline bool running = true;

	inline int screen_height;

	inline int screen_width;

	// TODO: Parse JSON config to populate actions map
	inline const std::unordered_map<xieite::keys, std::function<void()>> actions = {
		{ xieite::keys::ctrl_q, [] { nsn::running = false; } },
		{ xieite::keys::right, [] { nsn::file_pos += nsn::file_pos < nsn::file_buf.size(); } },
		{ xieite::keys::left, [] { nsn::file_pos -= !!nsn::file_pos; } },
		{ xieite::keys::down, [] { /* TODO */ } },
		{ xieite::keys::up, [] { /* TODO */ } }
	};
}
