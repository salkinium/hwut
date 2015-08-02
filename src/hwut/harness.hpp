/*
 * harness.hpp
 *
 * Copyright (c) 2009-2015, Roboterclub Aachen e.V.
 *
 * This file is part of hwut.
 *
 * hwut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * hwut is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hwut.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef	HWUT_HARNESS_HPP
#define	HWUT_HARNESS_HPP

#include "controller.hpp"

#define	TEST_FLOAT_EPISLON		0.00001f

#ifdef __DOXYGEN__

/**
 * \brief	Verify (expr) is true
 *
 * \ingroup	hwut
 */
#define	TEST_ASSERT_TRUE(expr)

/**
 * \brief	Verify (expr) is false
 *
 * \ingroup	hwut
 */
#define	TEST_ASSERT_FALSE(expr)

/**
 * \brief	Verify (x==y)
 *
 * Shortcut with extended output message for `TEST_ASSERT_TRUE(x == y);`
 *
 * \ingroup	hwut
 */
#define	TEST_ASSERT_EQUALS(x, y)

/**
 * \brief	Verify (x==y) for floating point values
 * \ingroup	hwut
 */
#define	TEST_ASSERT_EQUALS_FLOAT(x, y)

/**
 * \brief	Verify (x==y) up to d
 *
 * This macro verifies two values are equal up to a delta
 *
 * \ingroup	hwut
 */
#define	TEST_ASSERT_EQUALS_DELTA(x, y, d)

/**
 * \brief	Verify (lower <= value <= upper)
 * \ingroup	hwut
 */
#define	TEST_ASSERT_EQUALS_RANGE(value, lower, upper)

/**
 * \brief	Check if the arrays contains the same data
 *
 * start is optional (default = 0).
 *
 * \ingroup	hwut
 */
#define	TEST_ASSERT_EQUALS_ARRAY(array1, array2, count, start)

/**
 * \brief	Fail unconditionally
 * \ingroup	hwut
 */
#define	TEST_FAIL(msg)

#else // !__DOXYGEN__

#include <xpcc/architecture/driver/accessor/flash.hpp>

namespace hwut
{
	EXTERN_FLASH_STORAGE_STRING(typeAssertEquals);
	EXTERN_FLASH_STORAGE_STRING(typeAssertEqualsDelta);
	EXTERN_FLASH_STORAGE_STRING(typeAssertInRange);
	EXTERN_FLASH_STORAGE_STRING(typeAssertTrue);
	EXTERN_FLASH_STORAGE_STRING(typeAssertFalse);
	EXTERN_FLASH_STORAGE_STRING(typeAssertArray);

	EXTERN_FLASH_STORAGE_STRING(stringArgs);
	EXTERN_FLASH_STORAGE_STRING(stringValues);
	EXTERN_FLASH_STORAGE_STRING(stringComma);
	EXTERN_FLASH_STORAGE_STRING(stringMessage);
}

#ifdef	HWUT_RETURN_ON_FAIL
	#define	TEST_RETURN__(x)	do { if (!x) { return; } } while (0)
#else
	#define	TEST_RETURN__(x)	x
#endif

#define	TEST_REPORTER__		hwut::Controller::instance().getReporter()

namespace hwut
{
	// ------------------------------------------------------------------------
	bool
	checkExpression(bool expr, bool expected, unsigned int line, xpcc::accessor::Flash<char> arguments);

	// ------------------------------------------------------------------------
	bool
	checkEqual(const float& a, const float& b, unsigned int line, xpcc::accessor::Flash<char> arguments);

	// ------------------------------------------------------------------------
	template <typename A, typename B>
	bool
	checkEqual(const A& a, const B& b, unsigned int line, xpcc::accessor::Flash<char> arguments)
	{
		using namespace xpcc::accessor;
		bool pass = (a == b);

		TEST_REPORTER__.report(pass, line, asFlash(hwut::typeAssertEquals))
			<< asFlash(hwut::stringArgs)
			<< arguments
			<< asFlash(hwut::stringValues)
			<< a << asFlash(hwut::stringComma) << b
			<< '\'';

		return pass;
	}

	template <typename A, typename B, typename D>
	bool
	checkEqualDelta(const A& a, const B& b, const D& delta, unsigned int line, xpcc::accessor::Flash<char> arguments)
	{
		using namespace xpcc::accessor;
		bool pass = (((a + delta) >= b) and ((a - delta) <= b));

		TEST_REPORTER__.report(pass, line, asFlash(hwut::typeAssertEquals))
			<< asFlash(hwut::stringArgs)
			<< arguments
			<< asFlash(hwut::stringValues)
			<< a << asFlash(hwut::stringComma) << b << asFlash(hwut::stringComma) << delta
			<< '\'';

		return pass;
	}

	template <typename T, typename B>
	bool
	checkRange(const T& value, const B& lower, const B& upper, unsigned int line, xpcc::accessor::Flash<char> arguments)
	{
		using namespace xpcc::accessor;
		bool pass = ((value >= lower) and (value <= upper));

		TEST_REPORTER__.report(pass, line, asFlash(hwut::typeAssertEquals))
			<< asFlash(hwut::stringArgs)
			<< arguments
			<< asFlash(hwut::stringValues)
			<< value << asFlash(hwut::stringComma) << lower << asFlash(hwut::stringComma) << upper
			<< '\'';

		return pass;
	}

	// ------------------------------------------------------------------------
	template <typename T>
	inline void
	printArray(xpcc::IOStream& stream, unsigned int start, unsigned int count, const T& array)
	{
		using namespace xpcc::accessor;
		stream << '[';
		for (unsigned int i = start; i < (start + count); ++i) {
			stream << array[i];
			if (i != start + count - 1) {
				stream << asFlash(hwut::stringComma);
			}
		}
		stream << ']';
	}

	template <typename A, typename B>
	bool
	checkArray(const A& a, const B& b, unsigned int line, xpcc::accessor::Flash<char> arguments, std::size_t count, std::size_t start = 0)
	{
		using namespace xpcc::accessor;
		for (std::size_t i = start; i < (start + count); ++i)
		{
			if (a[i] != b[i])
			{
				xpcc::IOStream& stream = TEST_REPORTER__.report(false, line, asFlash(hwut::typeAssertArray));
				stream	<< asFlash(hwut::stringArgs)
						<< arguments
						<< asFlash(hwut::stringValues);

				printArray(stream, start, count, a);
				stream << asFlash(hwut::stringComma);
				printArray(stream, start, count, b);
				stream << '\'';
				return false;
			}
		}

		xpcc::IOStream& stream = TEST_REPORTER__.report(true, line, asFlash(hwut::typeAssertArray));
		stream	<< asFlash(hwut::stringArgs)
				<< arguments
				<< asFlash(hwut::stringValues);
		printArray(stream, start, count, a);
		stream << '\'';
		return true;
	}
}

#define	TEST_ASSERT_TRUE(expr)	\
	TEST_RETURN__(::hwut::checkExpression((expr), true, __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(expr) ) )  ))

#define	TEST_ASSERT_FALSE(expr)	\
	TEST_RETURN__(::hwut::checkExpression((expr), false,  __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(expr) ) )  ))

#define	TEST_ASSERT_EQUALS(x, y) \
	TEST_RETURN__(::hwut::checkEqual((x), (y), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) ", " STRINGIFY(y) ) )  ))

#define	TEST_ASSERT_EQUALS_FLOAT(x, y) \
	TEST_RETURN__(::hwut::checkEqual(static_cast<float>(x), static_cast<float>(y), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) ", " STRINGIFY(y) ) )  ))

#define	TEST_ASSERT_EQUALS_DELTA(x, y, d) \
	TEST_RETURN__(::hwut::checkEqualDelta((x), (y), (d), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) ", " STRINGIFY(y) ", " STRINGIFY(d)) )  ))

#define	TEST_ASSERT_EQUALS_RANGE(value, lower, upper) \
	TEST_RETURN__(::hwut::checkRange((value), (lower), (upper), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(value) ", " STRINGIFY(lower) ", " STRINGIFY(upper)) )  ))

#define	TEST_ASSERT_EQUALS_ARRAY(x, y, count, ...) \
	TEST_RETURN__(::hwut::checkArray((x), (y), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) ", " STRINGIFY(y) ", " STRINGIFY(count) )), count, \
		##__VA_ARGS__))

#define	TEST_FAIL(msg) \
	do {	TEST_REPORTER__.report(false, __LINE__, xpcc::accessor::asFlash(hwut::stringMessage)) \
			<< msg << '\''; \
	} while (0); \
	TEST_RETURN__((void) false)

#endif	// __DOXYGEN__

#endif	// HWUT_HARNESS_HPP
