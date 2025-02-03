#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "keys.hpp"

namespace nsn {
    inline nlohmann::json config_data;

    void load_config(const std::string path = "../nsn_config.json");
    [[nodiscard]] nsn::keys get_keybind(std::string action);
}