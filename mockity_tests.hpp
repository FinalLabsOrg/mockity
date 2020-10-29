// mockity_tests.hpp: use this file as an includde in your MS Tests Project
// A part of Mockity
/*

Copyright (c) 2020 FINAL Neural Architecture Laboratories

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

*/

#pragma once

#include <cstdlib>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef USING_MOCKITY
#define USING_MOCKITY
#endif

/*
Shorthand when icluding CppUnitTest.h
*/
#ifndef MOCKITY_MS_TEST_INCLUDE
#define MOCKITY_MS_TEST_INCLUDE "CppUnitTest.h"
#endif

/*
Shorthand for using the MS Test namespace
*/
#ifndef MOCKITY_USE_MS_TEST_NAMESPACE
#define MOCKITY_USE_MS_TEST_NAMESPACE using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

/*
Shorthand for a fixed random seed
*/
#define MOCKITY_SRAND srand(1);

/*
Stub for unimplemented tests
*/
#define MOCKITY_FAIL_STUB Assert::Fail(L"To be implemented");

#ifndef MOCKITY_INIT_STRINGER
/*
Calls Mockity::Stringer's constructor
*/
#define MOCKITY_INIT_STRINGER Mockity::Stringer()
#endif

#ifndef MOCKITY_WITH_STRINGER
/*
Provides for public inheritance from Stringer
*/
#define MOCKITY_WITH_STRINGER public Mockity::Stringer
#endif

/*
#ifndef MOCKITY_IGNORE
#define MOCKITY_IGNORE(TEST_TO_BE_IGNORED) BEGIN_TEST_METHOD_ATTRIBUTE(TEST_TO_BE_IGNORED) TEST_IGNORE() END_TEST_METHOD_ATTRIBUTE()
#endif
*/

namespace Mockity {

	/*

	Mockity::Stringer is Mockity's inspector helper class. It should be one of
	the parent classes of the mock classes you create. Stringer helps count
	function calls and assert calling order. 
	Currently, Mockity::Stringer is not thread-safe.

	Mockity::Stringer is named, in the footsteps of Dr. Watson, after Mr. Stringer,
	Miss Marple's aide in the Miss Marple films of the 1960s featuring Margaret
	Rutherford as Miss Marple and her husband, Stringer Davis, as Mr. Stringer.

	https://en.wikipedia.org/wiki/Stringer_Davis
	https://youtu.be/vnCCPMzXi5M

	*/
	class Stringer {
	private:
		std::shared_ptr<std::vector<std::string>> aActualOrder;
		std::shared_ptr<std::vector<std::string>> aExpectedOrder;
		std::shared_ptr<std::map<std::string, unsigned>> aFunctionCounter;
	public:
		inline Stringer();

		/// <summary>
		/// Returns true if the actual calling order is in line with the expectations.
		/// You can then simply Assert::IsTrue(pMockObject->assertOrder()).
		/// </summary>
		/// <returns>Bool: is the actual calling order in line with expectations?</returns>
		bool assertOrder() const { return *aExpectedOrder == *aActualOrder; }

		/// <summary>
		/// Used in the virtual method before calling the actual method in the parent class. 
		/// Just pass it the code/name of the current method call in order to count how many 
		/// times it is called.
		/// </summary>
		/// <param name="s">Code/name of the current method call</param>
		inline void countFunctionCall(std::string s) const;

		/// <summary>
		/// Call it from the test method when defining your expected call order. 
		/// Pass the code/name of the next expected method call to record the order 
		/// the methods are expected to be called.
		/// </summary>
		/// <param name="s">Code/name of the next expected method call</param>
		void expectOrder(std::string s) { aExpectedOrder->push_back(s); }

		/// <summary>
		/// Returns the number of calls to a specific code/method. 
		/// You can check this output as part of one of your assertions.
		/// </summary>
		/// <param name="s">Code/name of the method call in question</param>
		/// <returns>unsigned: the number of calls to a specific code/method</returns>
		inline unsigned getFunctionCallCount(std::string s) const;
	};

	Stringer::Stringer() {
		aFunctionCounter = std::make_shared<std::map<std::string, unsigned>>();
		aActualOrder = std::make_shared<std::vector<std::string>>();
		aExpectedOrder = std::make_shared<std::vector<std::string>>();
	}

	void Stringer::countFunctionCall(std::string s) const {
		aActualOrder->push_back(s);
		try {
			aFunctionCounter->at(s)++;
		}
		catch (std::out_of_range) {
			aFunctionCounter->insert_or_assign(s, 1);
		}
	}

	inline unsigned Stringer::getFunctionCallCount(std::string s) const
	{
		try {
			return aFunctionCounter->at(s);
		}
		catch (std::out_of_range) {
			return 0;
		}
	}

}