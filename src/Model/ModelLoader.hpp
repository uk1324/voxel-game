#pragma once

#include <Model/Model.hpp>
#include <Utils/Opt.hpp>

#include <string_view>
#include <vector>

class ModelLoader
{
private:


public:
	Opt<Model> parse(std::string_view path);

private:
	//std::vector<
};