#include "other/utils.hpp"

#include <iostream>


std::filesystem::path make_absolute_path(std::filesystem::path const& path, bool check_path_exists)
{
    auto const res {path.is_relative() ? CMAKE_SOURCE_DIR / path : path };

    if (check_path_exists && !std::filesystem::exists(res))
    {
        using namespace std::literals;
        auto const msg { "Path \""s + res.string() + "\" does not exist. Make sure you didn't misspell it or made it relative to the wrong folder. All relative paths will be relative to the directory containing your CMakeLists.txt file." };
        std::cerr << msg << '\n';
        throw std::runtime_error{msg};
    }

    return res;
}

std::vector<glm::u8vec3> pixels_as_vec3(img::Image const& image){
    if (image.channels_count() != 3)
    {
        throw std::runtime_error{"Image must have 3 channels"};
    }
    uint8_t const* data {image.data()};
    size_t const size {image.width() * image.height()};

    std::vector<glm::u8vec3> pixels {};
    pixels.reserve(size);
    for (size_t i {0}; i < size; ++i)
    {
        pixels.push_back({data[i * 3], data[i * 3 + 1], data[i * 3 + 2]});
    }
    return pixels;
}

int convertY(int y, int height){
    return height - y - 1;
}
