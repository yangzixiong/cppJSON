#include <iostream>
#include "cppJSON.h"
using namespace std;

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

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

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual)

static void test_parse_null() 
{
	CppJSON v;
	v.type = JSON_TRUE;
	EXPECT_EQ_INT(JSON_PARSE_OK, json_parse(&v, "null"));
	EXPECT_EQ_INT(JSON_NULL, json_get_type(&v));
}	

static void test_parse()
{
	test_parse_null();
}

int main()
{
	test_parse();
	cout<<test_pass<<'//'<<test_count<<(test_pass*100.0)/test_count<<"%"<<"passed";
	return main_ret;
}
