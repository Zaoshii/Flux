#include "Core/file/file.hpp"

namespace engine::core::file
{
std::string readTextFile(const std::filesystem::path &path)
{
    std::ifstream input(path, std::ios::in);

    if (!input.is_open())
    {
        throw std::runtime_error("Failed to open file" + path.string());
    }

    std::stringstream buffer;
    buffer << input.rdbuf();

    if (input.bad())
    {
        throw std::runtime_error("Failed to read file" + path.string());
    }

    return buffer.str();
}
} // namespace engine::core::file