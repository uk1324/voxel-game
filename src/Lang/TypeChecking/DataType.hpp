#pragma once

#include <string_view>

namespace Lang
{
	enum class DataTypeType
	{
		Dynamic,
		Int,
		Float,
		UnresolvedTypeName,
		Unspecified
	};

	const char* dataTypeTypeToString(DataTypeType type);

	struct DataType
	{
	public:
		DataType();
		DataType(DataTypeType type);
		//DataType(DataTypeType type, bool isConst = false)
		// ADD SIZE_t start and end

	public:
		DataTypeType type;

		union
		{
			std::string_view unresolvedTypeName;
		};
	};
}