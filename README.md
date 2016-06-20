## About ##

µTest is an ultra-lightweight micro unit test framework for C99. It is the sister framework of [µTest for C++11](https://github.com/evolutional/upptest).


## Compiling ##

µTest is provided as a single-header library. To compile, you simply include the header where you need to and ensure that you provide the implementation by defining `UTEST_C_IMPLEMENTATION` in **exactly one** source file before you include the header.

		#define UTEST_C_IMPLEMENTATION
		#include "utest.h"


## Usage ##

Before executing a test, call:

	utest_init();


Specify tests as functions with the signature: 
	`void [func_name](void)`

A convenience macro `TEST` has been defined for quick definition of tests

eg:

	 TEST(my_test)
	 {
		// do nothing
	 }

Tests can be run individually via `TEST_RUN()`

eg:
	
	int result = TEST_RUN(my_test);

A test fails if it returns a non-zero result. You can check the failure message by calling `utest_last_msg()`.


## Assertion ##

µTest provides several simple assertion macros for use in tests.

	// Fail the test
	TEST_FAIL()
	
	// Fail the test with a message		
	TEST_FAIL_MESSAGE(message)

	// Assert expr == true	
	TEST_ASSERT(expr)

	// Assert signed 8-bit int value actual == expected
	// There are INT8, INT16, INT32 and INT64 variants of this macro
	TEST_ASSERT_EQUAL_INT8(expected, actual)
	
	// Assert unsigned 8-bit int value actual == expected
	// There are UINT8, UINT16, UINT32 and UINT64 variants of this macro
	TEST_ASSERT_EQUAL_UINT8(expected, actual)

	// Assert float value actual is within epsilon of expected
	TEST_ASSERT_EQUAL_FLOAT(expected, actual, epsilon)
	TEST_ASSERT_EQUAL_DOUBLE(expected, actual, epsilon)

	// Assert string value actual == expected
	TEST_ASSERT_EQUAL_STRING(expected, actual)
	TEST_ASSERT_EQUAL_NOCASE_STRING(expected, actual)

	// Print an information message from this test (does not fail)
	TEST_MESSAGE(message) 

Note: The assert/fail macros do not raise asserts - instead they record the failure message and terminate the test.

## Fixtures ##


Often groups of related tests will be arranged into fixtures, using the macros below.

	TEST_FIXTURE_BEGIN(mytest_fixture)
		TEST_FIXTURE_TEST(mytest_a)
		TEST_FIXTURE_TEST(mytest_b)
		TEST_FIXTURE_TEST(mytest_c)
	TEST_FIXTURE_END()

Fixtures can be run by calling `TEST_RUN_FIXTURE()`

eg:
	
	int result = TEST_RUN_FIXTURE(mytest_fixture);


Fixtures support per-test setup/teardown functions that are called before/after each test execution respectively.
This is useful to perform any common initialization and cleanup logic that wraps each test. These functions have 
the same signature as a test (eg: `void [func_name](void)`).


To declare a fixture with a test setup/teardown method...

	void myfixture_test_setup()
	{
		// do work before test
	}

	void myfixture_test_teardown()
	{
		// do work after test
	}

	TEST_FIXTURE_BEGIN_F(mytest_fixture, myfixture_test_setup, myfixture_test_teardown)
		TEST_FIXTURE_TEST(mytest_a)
		TEST_FIXTURE_TEST(mytest_b)
		TEST_FIXTURE_TEST(mytest_c)
	TEST_FIXTURE_END()


Fixtures also support fixture-level setup/teardown functions that are run before/after any tests. This is useful To
perform any one time initialization and cleanup logic that needs to be run before anything else. These functions have 
the same signature as a test (eg: `void [func_name](void)`).


To declare a fixture with a fixture setup/teardown method...

	void myfixture_setup()
	{
		// do work before any tests
	}

	void myfixture_teardown()
	{
		// do work after any tests
	}

	TEST_FIXTURE_BEGIN_G(mytest_fixture, myfixture_setup, myfixture_teardown)
		TEST_FIXTURE_TEST(mytest_a)
		TEST_FIXTURE_TEST(mytest_b)
		TEST_FIXTURE_TEST(mytest_c)
	TEST_FIXTURE_END()


It is possible to combine both fixture setup and per-test setup functions by as follows:

	TEST_FIXTURE_BEGIN_A(mytest_fixture, myfixture_setup, myfixture_teardown, myfixture_test_setup, myfixture_test_teardown)
		TEST_FIXTURE_TEST(mytest_a)
		TEST_FIXTURE_TEST(mytest_b)
		TEST_FIXTURE_TEST(mytest_c)
	TEST_FIXTURE_END()


This will yield the execution flow detailed here:

	myfixture_setup()

		myfixture_test_setup()
			mytest_a()
		myfixture_test_teardown()

		myfixture_test_setup()
			mytest_b()
		myfixture_test_teardown()

		myfixture_test_setup()
			mytest_c()
		myfixture_test_teardown()

	myfixture_teardown()



## Configuration ##

Define `UTEST_MAX_TESTS_PER_FIXTURE` before including the header to change the number of tests allowed per fixture.

Define `UTEST_MSG_BUFFER_SIZE` to a different size if you need to change the size of the error message buffer.

## Utility ##

Use `utest_set_user(void*)` and `utest_get_user` to store and retrieve global data. This is useful for storing 
data to be used in a test.

You can hook the results of a test execution by passing a function with the signature `void(func)(const utest_fixture*, const utest_entry*, utest_result, const char*)`
to `utest_set_result_func`. You will receive information about the test, its parent fixture, the execution status and the assert message if it failed.
This is useful if you wish to embed µTest in your own application or test runner framework.

Similarly, you can hook the results of a test `TEST_MESSAGE` by passing a function with the signature `void(func)(const utest_entry*, const char*)`
to `utest_set_print_func`.

## Caveats ##

Tests execution is wrapped in [`setjmp`](http://en.cppreference.com/w/cpp/utility/program/setjmp) & [`longjmp`](http://en.cppreference.com/w/cpp/utility/program/longjmp). 
As a result it can cause issues with stack unwinding and destructors in C++ (especially if you're using RAII). Be aware of the potential issues of
this when writing tests and allocate any resources within test setup/teardown functions.

## Future ##

Planned features (in no order):

- More assert macros
- Usability improvements


## License ##

µTest is free and unencumbered software released into the public domain. See UNLICENSE for details.