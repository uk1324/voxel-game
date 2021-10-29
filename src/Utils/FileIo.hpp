#pragma once

#include <Json/Json.hpp>

#include <string>
#include <string_view>


std::string stringFromFile(std::string_view path);

Json::Value jsonFromFile(std::string_view path);