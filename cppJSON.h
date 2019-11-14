#ifndef CPPJSON_H__
#define CPPJSON_H__
#include <iostream>

using namespace std;
typedef enum
{
	JSON_PARSE_OK = 0,
	JSON_PARSE_EXPECT_VALUE = 1,//表示JSON中只含有空白
	JSON_PARSE_INVALID_VALUE = 2,//不是JSON类型值
	JSON_PARSE_ROOT_NOT_SINGULAR = 3//一个值之后，空白之后还有其他字符
}JSON_PARSE_RET;

typedef enum
{
	JSON_NULL = 0, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT
}json_type;

class CppJson
{
	
public:
	double n;
	json_type type;
	double json_get_number();
	json_type json_get_type();
};


class CppJsonContext
{
public:
	const char* json;
};

class CppJsonParser
{
public:
	JSON_PARSE_RET json_parse(CppJson * v, const char * json);
};














#endif 