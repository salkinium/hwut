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
	FLASH_STORAGE_STRING(stringEqual) = " == ";
	FLASH_STORAGE_STRING(stringDelta) = " ~ ";
	FLASH_STORAGE_STRING(stringNotInRange) = " not in range ";
	FLASH_STORAGE_STRING(stringFailBodyExpression) = "Expression not ";
	FLASH_STORAGE_STRING(stringFailBodyEqual) = " not equal";
	FLASH_STORAGE_STRING(stringFailBodyEqualFloat) = " with float eplison'\n";
	FLASH_STORAGE_STRING(stringFailBodyEqualDelta) = " with delta ";
	FLASH_STORAGE_STRING(stringFailBodyEnd) = "'\n";
}

bool
hwut::checkExpression(bool expr, bool expected, unsigned int line, xpcc::accessor::Flash<char> checkString)
{
	if (expr == expected) {
		TEST_REPORTER__.reportPass(checkString) << xpcc::endl;
		return true;
	} else {
		TEST_REPORTER__.reportFailure(checkString, line)
			<< xpcc::accessor::asFlash(hwut::stringFailBodyExpression)
			<< expected
			<< xpcc::accessor::asFlash(hwut::stringFailBodyEnd);
		return false;
	}
}

bool
hwut::checkEqual(const float& a, const float& b, unsigned int line, xpcc::accessor::Flash<char> checkString)
{
	if (((a + TEST_FLOAT_EPISLON) >= b) and ((a - TEST_FLOAT_EPISLON) <= b))
	{
		TEST_REPORTER__.reportPass(checkString) << xpcc::endl;
		return true;
	}
	else {
		TEST_REPORTER__.reportFailure(checkString, line)
			<< a << xpcc::accessor::asFlash(hwut::stringEqual) << b
				<< xpcc::accessor::asFlash(hwut::stringFailBodyEqual)
			<< xpcc::accessor::asFlash(hwut::stringFailBodyEqualFloat);
		return false;
	}
}
