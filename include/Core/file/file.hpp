#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace engine::core::file
{
std::string readTextFile(const std::filesystem::path &path);

} // namespace engine::core::file