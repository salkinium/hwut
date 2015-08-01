/*
 * reporter.cpp
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

#include "reporter.hpp"

namespace
{
	FLASH_STORAGE_STRING(invaildName) = "invalid";
	
	FLASH_STORAGE_STRING(failHeader) = "FAIL: ";
	FLASH_STORAGE_STRING(failColon) = " : ";
	
	FLASH_STORAGE_STRING(reportPassed) = "\nPassed ";
	FLASH_STORAGE_STRING(reportFailed) = "\nFailed ";
	FLASH_STORAGE_STRING(reportOf) = " of ";
	FLASH_STORAGE_STRING(reportTests) = " tests\n";
	FLASH_STORAGE_STRING(reportOk) = "OK!\n";
	FLASH_STORAGE_STRING(reportFail) = "FAIL!\n";
}

hwut::Reporter::Reporter(xpcc::IODevice& device) :
	outputStream(device), testName(xpcc::accessor::asFlash(invaildName)),
	testsPassed(0), testsFailed(0)
{
}

void
hwut::Reporter::nextTestSuite(xpcc::accessor::Flash<char> name)
{
	testName = name;
}

void
hwut::Reporter::reportPass()
{
	testsPassed++;
}

xpcc::IOStream&
hwut::Reporter::reportFailure(unsigned int lineNumber)
{
	testsFailed++;
	outputStream << xpcc::accessor::asFlash(failHeader)
				 << testName
				 << ':'
				 << lineNumber
				 << xpcc::accessor::asFlash(failColon);
	return outputStream;
}

void
hwut::Reporter::printSummary()
{
	if (testsFailed == 0) {
		outputStream << xpcc::accessor::asFlash(reportPassed)
					 << testsPassed
					 << xpcc::accessor::asFlash(reportTests)
					 << xpcc::accessor::asFlash(reportOk)
					 << xpcc::endl;
	}
	else {
		outputStream << xpcc::accessor::asFlash(reportFailed)
					 << testsFailed
					 << xpcc::accessor::asFlash(reportOf)
					 << (testsFailed + testsPassed)
					 << xpcc::accessor::asFlash(reportTests)
					 << xpcc::accessor::asFlash(reportFail)
					 << xpcc::endl;
	}
}
