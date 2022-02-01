#include <Model/ModelLoader.hpp>

Opt<Model> ModelLoader::parse(std::string_view path)
{
	Json::Value file = jsonFromFile(path);

	return std::nullopt;
}
