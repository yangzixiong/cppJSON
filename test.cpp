#include <iostream>
#include "cppJSON.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
//对错误信息的测试
#define TEST_ERROR(error, json) \
	do\
	{\
		CppJson v;\
		CppJsonParser cjp;\
		v.type = JSON_FALSE;\
		EXPECT_EQ_INT(error, cjp.json_parse(&v, json));\
		EXPECT_EQ_INT(JSON_NULL, v.json_get_type());\
	} while(0)

#define TEST_NUMBER(expect, json)\
	do\
	{\
		CppJson v;\
		CppJsonParser cjp;\
		EXPECT_EQ_INT(JSON_PARSE_OK, cjp.json_parse(&v, json));\
		EXPECT_EQ_INT(JSON_NUMBER, v.json_get_type());\
		EXPECT_EQ_DOUBLE(expect, v.json_get_number());\
	}while(0)

//调用测试函数
static void test_parse_null() 
{
	CppJson v;
	CppJsonParser cjp;
	v.type = JSON_TRUE;
	EXPECT_EQ_INT(JSON_PARSE_OK, cjp.json_parse(&v, "null"));
	EXPECT_EQ_INT(JSON_NULL, v.json_get_type());
}	
static void test_parse_expect_value()
{
	TEST_ERROR(JSON_PARSE_EXPECT_VALUE, "");
	TEST_ERROR(JSON_PARSE_EXPECT_VALUE, " ");
}
//一些非法数据的测试
static void test_parse_invalid_value()
{
	TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nul");
	TEST_ERROR(JSON_PARSE_INVALID_VALUE, "?");
	TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(LEPT_PARSE_INVALID_VALUE, "nan");
}
static void test_parse_root_not_singular()
{
	TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "null x");
}
static void test_parse_false()
{
	CppJson v;
	CppJsonParser cjp;
	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_OK, cjp.json_parse(&v, "false"));
	EXPECT_EQ_INT(JSON_FALSE, v.json_get_type());
}
static void test_parse_true()
{
	CppJson v;
	CppJsonParser cjp;
	v.type = JSON_FALSE;
	EXPECT_EQ_INT(JSON_PARSE_OK, cjp.json_parse(&v, "true"));
	EXPECT_EQ_INT(JSON_TRUE, v.json_get_type());
}
//测试合法number的解析
static void test_parse_number()
{
	TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

	//新增加的
    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
	TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
	TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
	TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
	TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
	TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
	TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
	TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
	TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
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
	test_parse_number();
	
}

//统计
int main()
{
	test_parse();
	cout<<test_pass<<"/"<<test_count<<" "<<(test_pass*100.0)/test_count<<"%"<<"passed";
	return main_ret;
}