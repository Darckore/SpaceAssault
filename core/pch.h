#pragma once

// std headers we'll most likely be using
#include <type_traits>
#include <concepts>
#include <utility>
#include <source_location>

#include <iostream>
#include <fstream>

#include <string>
#include <string_view>

#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include <optional>
#include <variant>

#include <filesystem>
#include <memory>
#include <stdexcept>

// just because it is often needed everywhere
using namespace std::literals;

#include "utils/utils.hpp"

namespace engine
{
  using time_type = float;
}