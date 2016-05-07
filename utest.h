/*
	utest - The micro unit test framework for C99
	
	Author - Oli Wilkinson (https://github.com/evolutional/utest)
 
	This file provides both the interface and the implementation.

	To provide the implementation for this library, you MUST define
	UTEST_C_IMPLEMENTATION in exactly on source file.

		#define UTEST_C_IMPLEMENTATION
		#include "utest.h"

	USAGE
	-----

	See README.md for details


	LICENSE
	-------

	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.

	For more information, please refer to <http://unlicense.org>
 */

#ifndef UTESTI_INCLUDE_UTEST_H_
#define UTESTI_INCLUDE_UTEST_H_

#include <stdint.h>
#include <setjmp.h> 

#ifndef UTEST_MAX_TESTS_PER_FIXTURE
#define UTEST_MAX_TESTS_PER_FIXTURE	265
#endif

#ifndef UTEST_MSG_BUFFER_SIZE
#define UTEST_MSG_BUFFER_SIZE	512
#endif

typedef void(*_utest_func)();

typedef struct _utest_entry
{
	// name of the test
	const char* name;

	// Function to run for the actual test
	_utest_func test;

	const char* file;
	const int line;

#ifdef __cplusplus
	_utest_entry() : name(0), test(0), file(0), line(0) {}
	_utest_entry(const char* a, _utest_func b, const char* c, const int d) : name(a), test(b), file(c), line(d) {}
#endif

} utest_entry;

typedef struct _utest_fixture
{
	// name of the fixture
	const char* name;

	// Fixture set up (run before any tests)
	_utest_func f_setup;
	// Fixture teardown (run after all tests)
	_utest_func f_teardown;

	// Set up (run before each test)
	_utest_func t_setup;
	// Teardown (run after each test)
	_utest_func t_teardown;

	utest_entry tests[UTEST_MAX_TESTS_PER_FIXTURE];
} utest_fixture;

typedef enum _utest_test_result
{
	TEST_RESULT_SUCCESS = 0,
	TEST_RESULT_FAILED = -1
} utest_test_result;

// handler called when a test completes (pass/fail)
typedef void(*utest_result_func)(const utest_fixture*, const utest_entry*, utest_test_result result, const char*);
typedef void(*utest_print_func)(const utest_entry*, const char*);

typedef struct _utest_state
{
	void*	user_state;

	int run_count;
	int pass_count;
	int fail_count;

	// working state
	utest_fixture* current_fixture;
	utest_entry* current_test;
	jmp_buf	restore_env;

	char msgbuffer[UTEST_MSG_BUFFER_SIZE];

} utest_state;

typedef struct _utest_cfg
{
	utest_result_func result_func;
	utest_print_func print_func;
} utest_cfg;


#ifdef __cplusplus
extern "C" {
#endif

extern void utest_init();

extern utest_test_result utest_run_fixture(utest_fixture* fixture);
extern utest_test_result utest_run_test(utest_entry* test);

// assert stuff
extern void utest_assert_integer_equal(signed expected, signed actual, const char* message);
extern void utest_assert_null(void* actual, char expect_null, const char* message);
extern void utest_assert_float_equal(double expected, double actual, double epsilon, const char* message);
extern void utest_assert_string_equal(const char* expected, const char* actual, const char* message, char ignore_case);
extern void utest_assert_pointer_equal(void* expected, void* actual, const char* message);

extern void utest_fail(const char* format, ...);


extern utest_state g_utest_state;
extern utest_cfg g_utest_cfg;

// Utility
extern void* utest_get_user();
extern void utest_set_user(void*);

extern void utest_test_result_func(const utest_fixture* fixture, const utest_entry* test, utest_test_result result, const char* message);
extern void utest_test_print_func(const utest_entry* test, const char* message);

extern void utest_set_result_func(utest_result_func func);
extern void utest_set_print_func(utest_print_func func);

extern void utest_reset_result_func();
extern void utest_reset_print_func();

extern const char* utest_last_msg();

#ifdef __cplusplus
}
#endif

/////////////
// Assert helpers
#define TEST_FAIL_MESSAGE(message)		utest_fail(message);
#define TEST_FAIL						TEST_FAIL_MESSAGE(0)

#define TEST_INCONCLUSIVE_MESSAGE(message)	utest_inconclusive(message);
#define TEST_INCONCLUSIVE					TEST_INCONCLUSIVE_MESSAGE(0)

#define TEST_MESSAGE(message)			utest_print(message);

#define TEST_ASSERT_MESSAGE(expr, message)	if(!(expr)) { TEST_FAIL_MESSAGE(message); }

#define TEST_ASSERT(expr)		TEST_ASSERT_MESSAGE(expr, "Expression is false")

#define TEST_ASSERT_EQUAL_P(expected, actual, message)	utest_assert_pointer_equal(expected, actual, message)
#define TEST_ASSERT_EQUAL_I(type, expected, actual, message)	utest_assert_integer_equal((type)expected, (type)actual, message)
#define TEST_ASSERT_EQUAL_F(type, expected, actual, epsilon, message)	utest_assert_float_equal((type)expected, (type)actual, epsilon, message)

#define TEST_ASSERT_EQUAL_INT(expected, actual)		TEST_ASSERT_EQUAL_I(signed int, expected, actual, 0)
#define TEST_ASSERT_EQUAL_INT8(expected, actual)	TEST_ASSERT_EQUAL_I(int8_t, expected, actual, 0)
#define TEST_ASSERT_EQUAL_INT16(expected, actual)	TEST_ASSERT_EQUAL_I(int16_t, expected, actual, 0)
#define TEST_ASSERT_EQUAL_INT32(expected, actual)	TEST_ASSERT_EQUAL_I(int32_t, expected, actual, 0)
#define TEST_ASSERT_EQUAL_INT64(expected, actual)	TEST_ASSERT_EQUAL_I(int64_t, expected, actual, 0)

#define TEST_ASSERT_EQUAL_UINT(expected, actual)	TEST_ASSERT_EQUAL_I(unsigned int, expected, actual, 0)
#define TEST_ASSERT_EQUAL_UINT8(expected, actual)	TEST_ASSERT_EQUAL_I(uint8_t, expected, actual, 0)
#define TEST_ASSERT_EQUAL_UINT16(expected, actual)	TEST_ASSERT_EQUAL_I(uint16_t, expected, actual, 0)
#define TEST_ASSERT_EQUAL_UINT32(expected, actual)	TEST_ASSERT_EQUAL_I(uint32_t, expected, actual, 0)
#define TEST_ASSERT_EQUAL_UINT64(expected, actual)	TEST_ASSERT_EQUAL_I(uint64_t, expected, actual, 0)

#define TEST_ASSERT_EQUAL_PTR(expected, actual)				TEST_ASSERT_EQUAL_P(expected, actual, 0)
#define TEST_ASSERT_EQUAL_PTR_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_P(expected, actual, message)

#define TEST_ASSERT_NULL(actual)		utest_assert_null(actual, 1, 0)
#define TEST_ASSERT_NULL_MESSAGE(actual, message)		utest_assert_null(actual, 1, message)

#define TEST_ASSERT_NOT_NULL(actual)		utest_assert_null(actual, 0, 0)
#define TEST_ASSERT_NOT_NULL_MESSAGE(actual, message)		utest_assert_null(actual, 0, message)

#define TEST_ASSERT_EQUAL_FLOAT(expected, actual, epsilon)	TEST_ASSERT_EQUAL_F(float, expected, actual, epsilon, 0)
#define TEST_ASSERT_EQUAL_DOUBLE(expected, actual, epsilon)	TEST_ASSERT_EQUAL_F(double, expected, actual, epsilon, 0)

#define TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(signed int, expected, actual, message)
#define TEST_ASSERT_EQUAL_INT8_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(int8_t, expected, actual, message)
#define TEST_ASSERT_EQUAL_INT16_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(int16_t, expected, actual, message)
#define TEST_ASSERT_EQUAL_INT32_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(int32_t, expected, actual, message)
#define TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(int64_t, expected, actual, message)

#define TEST_ASSERT_EQUAL_UINT_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(unsigned int, expected, actual, message)
#define TEST_ASSERT_EQUAL_UINT8_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(uint8_t, expected, actual, message)
#define TEST_ASSERT_EQUAL_UINT16_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(uint16_t, expected, actual, message)
#define TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(uint32_t, expected, actual, message)
#define TEST_ASSERT_EQUAL_UINT64_MESSAGE(expected, actual, message)	TEST_ASSERT_EQUAL_I(uint64_t, expected, actual, message)

#define TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, epsilon, message)	TEST_ASSERT_EQUAL_F(float, expected, actual, epsilon, message)
#define TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(expected, actual, epsilon, message)	TEST_ASSERT_EQUAL_F(double, expected, actual, epsilon, message)

#define TEST_ASSERT_EQUAL_NOCASE_STRING_MESSAGE(expected, actual, message)		utest_assert_string_equal(expected, actual, message, 1)
#define TEST_ASSERT_EQUAL_NOCASE_STRING(expected, actual)		TEST_ASSERT_EQUAL_NOCASE_STRING_MESSAGE(expected, actual, 0)

#define TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, message)		utest_assert_string_equal(expected, actual, message, 0)
#define TEST_ASSERT_EQUAL_STRING(expected, actual)		TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, 0)

//////////////
// Test specs
#define TEST(test_name)	\
	void test_name()

#define DECLARE_TEST(test_name)	\
	void test_name();

#define TEST_FIXTURE_BEGIN_A(fixture_name, fixture_setup, fixture_teardown, test_setup, test_teardown)	\
static utest_fixture fixture_name = { #fixture_name, fixture_setup, fixture_teardown, test_setup, test_teardown, {

#define TEST_FIXTURE_BEGIN_G(fixture_name, fixture_setup, fixture_teardown)	\
	TEST_FIXTURE_BEGIN_A(fixture_name, fixture_setup, fixture_teardown, 0, 0)

#define TEST_FIXTURE_BEGIN_F(fixture_name, test_setup, test_teardown)		\
	TEST_FIXTURE_BEGIN_A(fixture_name, 0, 0, test_setup, test_teardown)

#define TEST_FIXTURE_BEGIN(fixture_name)	TEST_FIXTURE_BEGIN_F(fixture_name, 0, 0)

// Adds a test to a fixture
#define TEST_FIXTURE_TEST(test_name)	\
			{ #test_name, test_name, __FILE__, __LINE__ },


#define TEST_FIXTURE_END()	\
			{ 0, 0, 0, 0 } \
		} \
	};

#define TEST_RUN(test_name)	\
	utest_run_test( &test_name )

#define TEST_RUN_FIXTURE(fixture_name)	\
	utest_run_fixture( &fixture_name )

#endif

#ifdef UTEST_C_IMPLEMENTATION

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#define __UTEST_FAIL_TEST_EXIT()		\
	longjmp(g_utest_state.restore_env, 1); \
	return;

// Global state
static utest_state g_utest_state;
static utest_cfg g_utest_cfg;

void utest_init()
{
	memset(&g_utest_state, 0, sizeof(g_utest_state));
	g_utest_cfg.result_func = &utest_test_result_func;
	g_utest_cfg.print_func = &utest_test_print_func;
}

void utest_print(const char* message)
{
	if (g_utest_cfg.print_func)
	{
		g_utest_cfg.print_func(g_utest_state.current_test, message);
	}	
}

utest_test_result utest_run_fixture(utest_fixture* fixture)
{
	utest_entry* test = fixture->tests;
	g_utest_state.run_count = 0;
	g_utest_state.pass_count = 0;
	g_utest_state.fail_count = 0;
	g_utest_state.current_fixture = fixture;

	if (fixture->f_setup)
	{
		(*fixture->f_setup)();
	}

	while (test->test)
	{
		g_utest_state.current_test = test;
		g_utest_state.run_count++;

		if (fixture->t_setup)
		{
			(*fixture->t_setup)();
		}

		utest_test_result result = utest_run_test(test++);

		if (fixture->t_teardown)
		{
			(*fixture->t_teardown)();
		}

		if (!result)
		{
			g_utest_state.pass_count++;
			g_utest_cfg.result_func(fixture, g_utest_state.current_test, result, 0);
		}
		else
		{
			g_utest_state.fail_count++;
			g_utest_cfg.result_func(fixture, g_utest_state.current_test, result, g_utest_state.msgbuffer);
		}
	}

	if (fixture->f_teardown)
	{
		(*fixture->f_teardown)();
	}

	return g_utest_state.fail_count ? TEST_RESULT_FAILED : TEST_RESULT_SUCCESS;
}

utest_test_result utest_run_test(utest_entry* test)
{
	// Grab current environment
	int result = setjmp(g_utest_state.restore_env);

	if (result == 0)
	{
		// Run the test
		(*test->test)();
		return TEST_RESULT_SUCCESS;
	}
	return TEST_RESULT_FAILED;
}

//////////////////////////
// Assert implementations
//
void utest_assert_integer_equal(signed expected, signed actual, const char* message)
{
	if (expected == actual)
	{
		return;
	}
	if (message)
	{
		utest_fail("Values not equal. Expected [%i], Actual [%i] - %s", expected, actual, message);
	}
	else
	{
		utest_fail("Values not equal. Expected [%i], Actual [%i]", expected, actual);
	}
}

void utest_assert_null(void* actual, char expect_null, const char* message)
{
	if (expect_null)
	{
		if(!actual)
		{
			return;
		}

		if (message)
		{
			utest_fail("Value non-null. Expected null. %s", message);
		}
		else
		{
			utest_fail("Value non-null. Expected null.");
		}
	}

	// otherwise we're expecting null
	if(actual)
	{
		return;
	}

	if (message)
	{
		utest_fail("Value null. Expected non-null. %s", message);
	}
	else
	{
		utest_fail("Value null. Expected non-null.");
	}
	
}

void utest_assert_float_equal(double expected, double actual, double epsilon, const char* message)
{
	if (fabs(expected - actual) < epsilon)
	{
		return;
	}

	if (message)
	{
		utest_fail("Values not equal. Expected [%lf], Actual [%lf] - %s", expected, actual, message);
	}
	else
	{
		utest_fail("Values not equal. Expected [%lf], Actual [%lf]", expected, actual);
	}
}

void utest_assert_string_equal(const char* expected, const char* actual, const char* message, char ignore_case)
{
	if (ignore_case)
	{
		if (_stricmp(expected, actual) == 0)
		{
			return;
		}
	}
	else
	{
		if (strcmp(expected, actual) == 0)
		{
			return;
		}
	}	

	if (message)
	{
		utest_fail("Strings not equal. Expected [%s], Actual [%s] - %s", expected, actual, message);
	}
	else
	{
		utest_fail("Strings not equal. Expected [%s], Actual [%s]", expected, actual);
	}
}

void utest_assert_pointer_equal(void* expected, void* actual, const char* message)
{
	if (expected == actual)
	{
		return;
	}

	if (message)
	{
		utest_fail("Pointers not equal. Expected [0x%016x], Actual [0x%016x] - %s", expected, actual, message);
	}
	else
	{
		utest_fail("Pointers not equal. Expected [0x%016x], Actual [0x%016x]", expected, actual);
	}
}

void utest_fail(const char* format, ...)
{
	va_list args;
	va_start(args, format);
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
	vsnprintf_s(g_utest_state.msgbuffer, UTEST_MSG_BUFFER_SIZE, UTEST_MSG_BUFFER_SIZE, format, args);
#else
	vsprintf(g_utest_state.msgbuffer, format, args);
#endif		
	va_end(args);	
	__UTEST_FAIL_TEST_EXIT();
}

void utest_test_result_func(const utest_fixture* fixture, const utest_entry* test, utest_test_result result, const char* message)
{
	if (result == TEST_RESULT_FAILED)
	{
		printf("%s(%i): Test failed: '%s': %s\r\n",
			test->file, test->line,
			test->name, message);
	}
}

void utest_test_print_func(const utest_entry* test, const char* message)
{
	printf("%s(%i): '%s': %s\r\n",
		test->file, test->line,
		test->name, message);
}

void* utest_get_user()
{
	return g_utest_state.user_state;
}

void utest_set_user(void* state)
{
	g_utest_state.user_state = state;
}

void utest_set_result_func(utest_result_func func)
{
	g_utest_cfg.result_func = func;
}

void utest_set_print_func(utest_print_func func)
{
	g_utest_cfg.print_func = func;
}

void utest_reset_result_func()
{
	g_utest_cfg.result_func = utest_test_result_func;
}

void utest_reset_print_func()
{
	g_utest_cfg.print_func = utest_test_print_func;
}

const char* utest_last_msg()
{
	return g_utest_state.msgbuffer;
}

#endif // UTEST_C_IMPLEMENTATION