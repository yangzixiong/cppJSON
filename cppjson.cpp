#include <iostream>
#include <string>
#include "cppJSON.h"

using namespace std;

#define EXPECT(c, ch) do { assert(*(c->json) == (ch)); c->json++;} while(0)


typedef enum
{
	JSON_PARSE_OK = 0,
	JSON_PARSE_EXPECT_VALUE,//表示JSON中只含有空白
	JSON_PARSE_INVALID_VALUE,//不是JSON类型值
	JSON_PARSE_ROOT_NOT_SINGULAR//一个值之后，空白之后还有其他字符
}JSON_PARSE_RET;

//跳过前后空格
//json-text = ws value ws
static JSON_PARSE_RET json_parse(CppJson * v, const char * json)
{
	CppJsonContext c;
	JSON_PARSE_RET ret;
	assert(v != NULL);
	c.json = json;
	v->type = JSON_NULL;
	json_parse_whitespace(&c);
	if((ret==json_parse_value(&c, v))==JSON_PARSE_OK)
	{
		json_parse_whitespace(&c);
		if(*(c.json) != '\0')
		{
			ret = JSON_PARSE_ROOT_NOT_SINGULAR;
		}
	}
	return ret;
}

//跳过空白
static void json_parse_whitespace(CppJsonContext * c)
{
	const char* p = c->json;
	while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
		p++;
	c->json = p;
}
//解析null
static JSON_PARSE_RET json_parse_null(CppJsonContext * c, CppJson * v)
{
	EXPECT(c, 'n');
	if(c->json[1] != 'u' || c->json[2] != 'l' || c->json[3] != 'l')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = JSON_NULL;
	return JSON_PARSE_OK;
}
//解析false
static JSON_PARSE_RET json_parse_false(CppJsonContext * c, CppJson * v)
{
	EXPECT(c, 'f');
	if(c->json[1] != 'a' || c->json[2] != 'l' || c->json[3] != 's' || c->json[4] != 'e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 4;
	v->type = CppJson.JSON_FALSE;
	return JSON_PARSE_OK;
}
//解析true
static JSON_PARSE_RET json_parse_true(CppJsonContext * c, CppJson * v)
{
	EXPECT(c, 't');
	if(c->json[1] != 'r' || c->json[2] != 'u' || c->json[3] != 'e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = CppJson.JSON_TRUE;
	return JSON_PARSE_OK;
}


static JSON_PARSE_RET json_parse_value(CppJsonContext * c, CppJson * v)
{
	switch(*(c->json))
	{
		case 'n': return json_parse_null(c, v);
		case 'f': return json_parse_false(c, v);
		case 't': return json_parse_true(c, v);
		case '\0': return JSON_PARSE_EXPECT_VALUE;
		default: return JSON_PARSE_INVALID_VALUE;
	}
}
