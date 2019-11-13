#ifndef CPPJSON_H__
#define CPPJSON_H__
#include <iostream>

using namespace std;


class CppJson
{
	typedef enum
	{
		JSON_NULL, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT
	}json_type;
public:
	json_type type;
}


class CppJsonContext{
public:
	const char* json;
}














#endif 
