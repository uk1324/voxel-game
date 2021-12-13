#pragma once

namespace Lang
{
	enum class DataTypeType
	{
		Dynamic,
		Int,
		Float,
		UnresolvedTypeName
	};

	const char* dataTypeTypeToString(DataTypeType type);

	struct DataType
	{
	public:
		DataType();
		DataType(DataTypeType type);
		//DataType(DataTypeType type, bool isConst = false)

	public:
		DataTypeType type;
	};
}