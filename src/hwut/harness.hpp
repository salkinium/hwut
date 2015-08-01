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
	EXTERN_FLASH_STORAGE_STRING(stringEqual);
	EXTERN_FLASH_STORAGE_STRING(stringDelta);
	EXTERN_FLASH_STORAGE_STRING(stringNotInRange);
	EXTERN_FLASH_STORAGE_STRING(stringFailBodyExpression);
	EXTERN_FLASH_STORAGE_STRING(stringFailBodyEqual);
	EXTERN_FLASH_STORAGE_STRING(stringFailBodyEqualFloat);
	EXTERN_FLASH_STORAGE_STRING(stringFailBodyEqualDelta);
	EXTERN_FLASH_STORAGE_STRING(stringFailBodyEnd);
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
	checkExpression(bool expr, bool expected, unsigned int line, xpcc::accessor::Flash<char> checkString);

	// ------------------------------------------------------------------------
	bool
	checkEqual(const float& a, const float& b, unsigned int line, xpcc::accessor::Flash<char> checkString);

	// ------------------------------------------------------------------------
	template <typename A, typename B>
	bool
	checkEqual(const A& a, const B& b, unsigned int line, xpcc::accessor::Flash<char> checkString)
	{
		if (a == b) {
			TEST_REPORTER__.reportPass(checkString) << xpcc::endl;
			return true;
		}
		else {
			TEST_REPORTER__.reportFailure(checkString, line)
				<< a << xpcc::accessor::asFlash(hwut::stringEqual) << b
				<< xpcc::accessor::asFlash(hwut::stringFailBodyEqual)
				<< xpcc::accessor::asFlash(hwut::stringFailBodyEnd);
			return false;
		}
	}

	template <typename A, typename B, typename D>
	bool
	checkEqualDelta(const A& a, const B& b, const D& delta, unsigned int line, xpcc::accessor::Flash<char> checkString)
	{
		if (((a + delta) >= b) and ((a - delta) <= b))
		{
			TEST_REPORTER__.reportPass(checkString) << xpcc::endl;
			return true;
		}
		else {
			TEST_REPORTER__.reportFailure(checkString, line)
				<< a << xpcc::accessor::asFlash(hwut::stringEqual) << b
				<< xpcc::accessor::asFlash(hwut::stringFailBodyEqual)
				<< xpcc::accessor::asFlash(hwut::stringFailBodyEqualDelta) << delta
				<< xpcc::accessor::asFlash(hwut::stringFailBodyEnd);
			return false;
		}
	}

	template <typename T, typename B>
	bool
	checkRange(const T& value, const B& lower, const B& upper, unsigned int line, xpcc::accessor::Flash<char> checkString)
	{
		if ((value >= lower) && (value <= upper))
		{
			TEST_REPORTER__.reportPass(checkString) << xpcc::endl;
			return true;
		}
		else {
			TEST_REPORTER__.reportFailure(checkString, line)
				<< value << xpcc::accessor::asFlash(hwut::stringNotInRange)
				<< '[' << lower << ',' << upper << ']'
				<< xpcc::accessor::asFlash(hwut::stringFailBodyEnd);
			return false;
		}
	}

	// ------------------------------------------------------------------------
	template <typename T>
	inline void
	printArray(xpcc::IOStream& stream, unsigned int start, unsigned int count, const T& array)
	{
		stream << '[';
		for (unsigned int i = start; i < (start + count); ++i) {
			stream << array[i];
			if (i != start + count - 1) {
				stream << ',' << ' ';
			}
		}
		stream << ']' << '\n';
	}

	template <typename A, typename B>
	bool
	checkArray(const A& a, const B& b, unsigned int line, xpcc::accessor::Flash<char> checkString, std::size_t count, std::size_t start = 0)
	{
		for (std::size_t i = start; i < (start + count); ++i)
		{
			if (a[i] != b[i])
			{
				xpcc::IOStream& stream = TEST_REPORTER__.reportFailure(checkString, line);

				stream << '\n';

				printArray(stream, start, count, a);
				printArray(stream, start, count, b);
				stream << xpcc::accessor::asFlash(hwut::stringFailBodyEnd);
				return false;
			}
		}

		TEST_REPORTER__.reportPass(checkString) << xpcc::endl;
		return true;
	}
}

#define	TEST_ASSERT_TRUE(expr)	\
	TEST_RETURN__(::hwut::checkExpression((expr), true, __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(expr) ) )  ))

#define	TEST_ASSERT_FALSE(expr)	\
	TEST_RETURN__(::hwut::checkExpression((expr), false,  __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( "!(" STRINGIFY(expr) ")" ) )  ))

#define	TEST_ASSERT_EQUALS(x, y) \
	TEST_RETURN__(::hwut::checkEqual((x), (y), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) " == " STRINGIFY(y) ) )  ))

#define	TEST_ASSERT_EQUALS_FLOAT(x, y) \
	TEST_RETURN__(::hwut::checkEqual(static_cast<float>(x), static_cast<float>(y), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) " == " STRINGIFY(y) ) )  ))

#define	TEST_ASSERT_EQUALS_DELTA(x, y, d) \
	TEST_RETURN__(::hwut::checkEqualDelta((x), (y), (d), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) " == " STRINGIFY(y) " ~ " STRINGIFY(d)) )  ))

#define	TEST_ASSERT_EQUALS_RANGE(value, lower, upper) \
	TEST_RETURN__(::hwut::checkRange((value), (lower), (upper), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(value) " in [" STRINGIFY(lower) ", " STRINGIFY(upper) "]") )  ))

#define	TEST_ASSERT_EQUALS_ARRAY(x, y, count, ...) \
	TEST_RETURN__(::hwut::checkArray((x), (y), __LINE__, \
		xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING( STRINGIFY(x) " == " STRINGIFY(y) " L=" STRINGIFY(count) )), count, \
		##__VA_ARGS__))

#define	TEST_FAIL(msg) \
	do {	TEST_REPORTER__.reportFailure(xpcc::accessor::asFlash( INLINE_FLASH_STORAGE_STRING("")), __LINE__) \
			<< msg << '\n'; \
	} while (0); \
	TEST_RETURN__((void) false)

#endif	// __DOXYGEN__

#endif	// HWUT_HARNESS_HPP
