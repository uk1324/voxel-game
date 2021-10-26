#include <gtest/gtest.h>

#include <Json/JsonPrinter.hpp>
#include <Json/JsonParser.hpp>

TEST(JsonPrinterTest, simpleJson)
{
	EXPECT_EQ(Json::stringify(5.75), "5.75");
	EXPECT_EQ(Json::stringify(126), "126");
	EXPECT_EQ(Json::stringify("abc"), "\"abc\"");
	EXPECT_EQ(Json::stringify("\na\tb"), "\"\\na\\tb\"");
	EXPECT_EQ(Json::stringify(true), "true");
	EXPECT_EQ(Json::stringify(false), "false");
	EXPECT_EQ(Json::stringify(nullptr), "null");
	EXPECT_EQ(Json::stringify(true), "true");
	EXPECT_EQ(Json::stringify({ 1, 2, 3, 4 }), "[1,2,3,4]");
	EXPECT_EQ(Json::stringify({ { "key0", "value0" } }), "{\"key0\":\"value0\"}");
}

// Cannot just compare strings because that would rely on ordering of Json::Value::MapType
TEST(JsonPrinterTest, complexJson)
{
	Json::Value json = {
		{"pi", 3.5},
		{"happy", true},
		{"name", "Niels"},
		{"nothing", nullptr},
		{"answer", {
			{"everything", 42}
		}},
		{"list", {1, 0, 2}},
		{"object", {
			{"currency", "USD"},
			{"value", 42.75}
		}}
	};

	json = Json::parse(Json::stringify(json));

	ASSERT_TRUE(json["pi"].isFloat());
	EXPECT_EQ(json["pi"].floatNumber(), 3.5);

	ASSERT_TRUE(json["happy"].isBoolean());
	EXPECT_EQ(json["happy"].boolean(), true);

	ASSERT_TRUE(json["name"].isString());
	EXPECT_EQ(json["name"].string(), "Niels");

	ASSERT_TRUE(json["nothing"].isNull());

	ASSERT_TRUE(json["answer"].isObject());
	ASSERT_TRUE(json["answer"]["everything"].isInt());
	EXPECT_EQ(json["answer"]["everything"].intNumber(), 42);

	ASSERT_TRUE(json["list"].isArray());
	ASSERT_TRUE(json["list"].array()[0].isInt());
	EXPECT_EQ(json["list"].array()[0].intNumber(), 1);
	ASSERT_TRUE(json["list"].array()[1].isInt());
	EXPECT_EQ(json["list"].array()[1].intNumber(), 0);
	ASSERT_TRUE(json["list"].array()[2].isInt());
	EXPECT_EQ(json["list"].array()[2].intNumber(), 2);

	ASSERT_TRUE(json["object"].isObject());
	ASSERT_TRUE(json["object"]["currency"].isString());
	EXPECT_EQ(json["object"]["currency"].string(), "USD");
	ASSERT_TRUE(json["object"]["value"].isFloat());
	EXPECT_EQ(json["object"]["value"].floatNumber(), 42.75);
}