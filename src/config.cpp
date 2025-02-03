#include "config.hpp"

#include <fstream>
#include <print>

void nsn::load_config(const std::string path) {
    std::ifstream file {path};
    config_data = nlohmann::json::parse(file);
}

nsn::keys nsn::get_keybind(std::string action) {
    return nsn::key_strings.at(nsn::config_data["keybinds"][action]);
}

nsn::keys nsn::get_keybind(std::string subset, std::string action) {
    return nsn::key_strings.at(nsn::config_data["keybinds"][subset][action]);
}
