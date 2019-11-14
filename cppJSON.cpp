#include <iostream>
#include <string>
#include <assert.h>
#include <cstdlib>
#include <string.h>
#include <typeinfo>
#include <cerror>
#include <cmath>
#include "cppJSON.h"

using namespace std;

#define EXPECT(c, ch) do { assert(*(c->json) == (ch)); c->json++;} while(0)
#define ISDIGIT1TO9(ch) do { ((ch) >= '1' && (ch) <= '9') } while(0)
#define ISDIGIT(ch) do { ((ch) >= '0' && (ch) <= '9') } while(0)

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
	if(c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
		return JSON_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = JSON_NULL;
	return JSON_PARSE_OK;
}
//解析false
static JSON_PARSE_RET json_parse_false(CppJsonContext * c, CppJson * v)
{
	EXPECT(c, 'f');
	if(c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
		return JSON_PARSE_INVALID_VALUE;
	c->json += 4;
	v->type = JSON_FALSE;
	return JSON_PARSE_OK;
}
//解析true
static JSON_PARSE_RET json_parse_true(CppJsonContext * c, CppJson * v)
{
	EXPECT(c, 't');
	if(c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
		return JSON_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = JSON_TRUE;
	return JSON_PARSE_OK;
}
//parse_null\parse_false\parse_true合成一个函数parse_literal
static JSON_PARSE_RET json_parse_literal(CppJsonContext * c, CppJson * v, const char * str, int len, json_type type)
{
	EXPECT(c, *str);
	str++;
	for(int i = 0; i < len-1; i++)
	{
		if(c->json[i] != *str++)
			return JSON_PARSE_INVALID_VALUE;
	}
	c->json += len-1;
	v->type = type;
	return JSON_PARSE_OK;
}
/*
number = [ "-" ] int [ frac ] [ exp ]
int = "0" / digit1-9 *digit
frac = "." 1*digit
exp = ("e" / "E") ["-" / "+"] 1*digit
*/
#if 0
static JSON_PARSE_RET json_parse_number(CppJsonContext * c, CppJson * v)
{
	char *end;
	//借用strtod函数
	v.n = strtod(c->json, &end);
	//没有数字
	if(c->json == end)
	{
		return JSON_PARSE_INVALID_VALUE;
	}
	c->json = end;
	v.type = JSON_NUMBER;
	return JSON_PARSE_OK;
}
#endif

static JSON_PARSE_RET json_parse_number(CppJsonContext * c, CppJson * v)
{
	const char* p = c->json;
	if(*p == '-') p++;
	if(*p == '0') p++;
	else
	{
		if(!ISDIGIT1TO9(*p)) return JSON_PARSE_INVALID_VALUE;
		for(p++; ISDIGIT(*p); p++);
	}
	if(*p == '.')
	{
		p++;
		if(!ISDIGIT(*p)) return JSON_PARSE_INVALID_VALUE;
		for(p++; ISDIGIT(*p); p++);
	}
	if(*p == 'e' || *p == 'E')
	{
		p++;
		if(*p != '+' && *p != '-') return JSON_PARSE_INVALID_VALUE;
		if(!ISDIGIT(*p)) return JSON_PARSE_INVALID_VALUE;
		for(p++; ISDIGIT(*p); p++);
	}

	//数值过大处理
	errno = 0;
	v.n = strtod(c->json, NULL);
    if (errno == ERANGE && v->n == HUGE_VAL) return JSON_PARSE_NUMBER_TOO_BIG;
	//
	v.type = JSON_NUMBER;
	c->json = p;
	return JSON_PARSE_OK;
}


static JSON_PARSE_RET json_parse_value(CppJsonContext * c, CppJson * v)
{
	// cout<<*(c->json)<<" "<<*c->json<<endl;
	// cout<<typeid(*(c->json)).name()<<endl;
	// bool b = *(c->json) == 'n';
	// cout<<b<<endl;
	switch(*(c->json))
	{
		case 'n': return json_parse_literal(c, v, "null", 4, JSON_NULL);
		case 'f': return json_parse_literal(c, v, "false", 5, JSON_FALSE);
		case 't': return json_parse_literal(c, v, "true", 4, JSON_TRUE);
		default: return json_parse_number(c, v);
		case '\0': return JSON_PARSE_EXPECT_VALUE;
	}
}



json_type CppJson::json_get_type()
{
	return this->type;
}
double CppJson::json_get_number()
{
	assert(this->type==JSON_NUMBER);
	return this->n;
}

//跳过前后空格
//json-text = ws value ws
JSON_PARSE_RET CppJsonParser::json_parse(CppJson * v, const char * json)
{
	CppJsonContext c;
	JSON_PARSE_RET ret;
	assert(v != NULL);
	c.json = json;
	v->type = JSON_NULL;
	//前面的空白
	json_parse_whitespace(&c);
	//解析
	ret=json_parse_value(&c, v);
	if(ret==JSON_PARSE_OK)
	{
		//后面的空白
		json_parse_whitespace(&c);
		if(*(c.json) != '\0')
		{
			ret = JSON_PARSE_ROOT_NOT_SINGULAR;
		}
	}
	return ret;
}