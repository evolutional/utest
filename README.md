## About ##

µTest is an ultra-lightweight micro unit test framework for C99.


## Compiling ##

µTest is provided as a single-header library. To compile, you simply include the header where you need to and ensure that you provide the implementation by defining `UTEST_C_IMPLEMENTATION` in **exactly one** source file before you include the header.

		#define UTEST_C_IMPLEMENTATION
		#include "utest.h"


## Usage ##


All tests must have the signature: 
	`void [func_name](void)`

A convenience macro `TEST` has been defined for quick definition of tests

eg:

	 TEST(my_test)
	 {
		// do nothing
	 }

Tests can be run individually via `utest_run_test()`

eg:
	
	int result = utest_run_test(my_test);


## Assertion ##

µTest provides several simple assertion macros for use in tests.

	// Fail the test
	TEST_FAIL()		

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




## Fixtures ##


Tests can be arranged into fixtures, using the macros below.

	TEST_FIXTURE_BEGIN(mytest_fixture)
		TEST_FIXTURE_TEST(mytest_a)
		TEST_FIXTURE_TEST(mytest_b)
		TEST_FIXTURE_TEST(mytest_c)
	TEST_FIXTURE_END()

Fixtures can be run by calling `utest_run_fixture()`

eg:
	
	int result = utest_run_fixture(mytest_fixture);


Fixtures support setup/teardown functions with the same signature as a
test. Setup/teardown functions are called before/after each test execution respectively.


To declare a test fixture with a setup/teardown method...

	void myfixture_setup()
	{
	}

	void myfixture_teardown()
	{
	}

	TEST_FIXTURE_BEGIN_F(mytest_fixture, myfixture_setup, myfixture_teardown)
		TEST_FIXTURE_TEST(mytest_a)
		TEST_FIXTURE_TEST(mytest_b)
		TEST_FIXTURE_TEST(mytest_c)
	TEST_FIXTURE_END()


## Configuration ##

Define `UTEST_MAX_TESTS_PER_FIXTURE` before including the header to change the number of tests allowed per fixture.

Define `UTEST_MSG_BUFFER_SIZE` to a different size if you need to change the size of the error message buffer.


## Future ##

Planned features (in no order):

- Ensure C++ support
- Custom error reporting (replace use of printf for output)
- More assert macros
- Usability improvements


## License ##

µTest is released under the MIT License (MIT). See LICENSE for details