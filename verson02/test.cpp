#include <iostream>
#include "cppJSON.h"
using namespace std;

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

//测试结果的处理
#define EXPECT_EQ_BASE(equality, expect, actual)\
	do{\
		test_count++;\
		if(equality)\
			test_pass++;\
		else\
		{\
			cerr<<__FILE__<<':'<<__LINE__<<"expect:"<<expect<<"actual:"<<actual<<endl;\
			main_ret = 1;\
		}\
	} while(0)
//测试结果
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual)

//调用测试函数
static void test_parse_null() 
{
	CppJSON v;
	v.type = JSON_TRUE;
	EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null"));
	EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
}	
static void test_parse_expect_value()
{
	CppJSON v;
	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_EXPECT_VALUE, json_parse(&v, ""));
	EXPECT_EQ_INT(LEPT_NULL, json_get_type(&v));

	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_EXPECT_VALUE, json_parse(&v, " "));
	EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
}
static void test_parse_invalid_value()
{
	CppJSON v;
	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_INVALID_VALUE, json_parse(&v, "nul"));
	EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));

	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_INVALID_VALUE, json_parse(&v, '?'));
	EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
}
static void test_parse_root_not_singular()
{
	CppJSON v;
	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_ROOT_NOT_SINGULAR, json_parse(&v, "null x"));
	EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
}
static void test_parse_false()
{
	CppJSON v;
	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "false"));
	EXPECT_EQ_INT(JSON_FALSE, json_get_type(&v));
}
static void test_parse_true()
{
	CppJSON v;
	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "true"));
	EXPECT_EQ_INT(JSON_TRUE, json_get_type(&v));
}
//调用所有测试函数
static void test_parse()
{
	test_parse_null();
	test_parse_true();
	test_parse_false();
	test_parse_expect_value();
	test_parse_invalid_value();
	test_parse_root_not_singular();
}

//统计
int main()
{
	test_parse();
	cout<<test_pass<<'//'<<test_count<<(test_pass*100.0)/test_count<<"%"<<"passed";
	return main_ret;
}
