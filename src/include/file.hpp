#pragma once
#include <filesystem>


std::string load_str_File(std::filesystem::path const& path);
bool isValid_ITD(std::string ITDText);
