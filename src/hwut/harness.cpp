/*
 * harness.cpp
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


#include "harness.hpp"

namespace hwut
{
	FLASH_STORAGE_STRING(typeAssertEquals) = "assert equals";
	FLASH_STORAGE_STRING(typeAssertEqualsDelta) = "assert equals delta";
	FLASH_STORAGE_STRING(typeAssertInRange) = "assert in range";
	FLASH_STORAGE_STRING(typeAssertTrue) = "assert true";
	FLASH_STORAGE_STRING(typeAssertFalse) = "assert false";
	FLASH_STORAGE_STRING(typeAssertArray) = "assert array";

	FLASH_STORAGE_STRING(stringArgs) = "\n\targs: '";
	FLASH_STORAGE_STRING(stringValues) = "'\n\tvalues: '";
	FLASH_STORAGE_STRING(stringComma) = ", ";

	FLASH_STORAGE_STRING(stringMessage) = "fail message'\n\tmessage: '";
}

using namespace xpcc::accessor;

bool
hwut::checkExpression(bool expr, bool expected, unsigned int line, xpcc::accessor::Flash<char> argument)
{
	bool pass = (expr == expected);

	TEST_REPORTER__.report(pass, line, asFlash(expected ? hwut::typeAssertTrue : hwut::typeAssertFalse))
		<< asFlash(hwut::stringArgs)
		<< argument
		<< asFlash(hwut::stringValues)
		<< expr
		<< '\'';

	return pass;
}

bool
hwut::checkEqual(const float& a, const float& b, unsigned int line, xpcc::accessor::Flash<char> argument)
{
	bool pass = (((a + TEST_FLOAT_EPISLON) >= b) and ((a - TEST_FLOAT_EPISLON) <= b));

	TEST_REPORTER__.report(pass, line, asFlash(hwut::typeAssertEquals))
		<< asFlash(hwut::stringArgs)
		<< argument
		<< asFlash(hwut::stringValues)
		<< a << asFlash(hwut::stringComma) << b
		<< '\'';

	return pass;
}
