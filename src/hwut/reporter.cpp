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
	FLASH_STORAGE_STRING(tapVersion) = "TAP version 13\n";
	FLASH_STORAGE_STRING(tapDate) = "# Unittests (" __DATE__ ", " __TIME__")\n";
	FLASH_STORAGE_STRING(tapTests) = "1..";
	FLASH_STORAGE_STRING(tapHash) = "# ";
	FLASH_STORAGE_STRING(tapDash) = " - ";
	FLASH_STORAGE_STRING(tapFailLine) = "\n\t---\n\tline: ";
	FLASH_STORAGE_STRING(tapFailMessage) = "\n\tmessage: '";

	FLASH_STORAGE_STRING(tapNotOk) = "not ok ";
	FLASH_STORAGE_STRING(tapOk) = "ok ";

	FLASH_STORAGE_STRING(reportListFailures) = "\nFAILED tests 1, N, TODO";
	FLASH_STORAGE_STRING(reportPassed) = "\nPassed ";
	FLASH_STORAGE_STRING(reportFailed) = "\nFailed ";
	FLASH_STORAGE_STRING(reportTests) = " tests, ";
	FLASH_STORAGE_STRING(reportPercent) = "\% okay";

	FLASH_STORAGE_STRING(invalidName) = "Invalid";
}

hwut::Reporter::Reporter(xpcc::IODevice& device) :
	outputStream(device), testName(xpcc::accessor::asFlash(invalidName)),
	testsPassed(0), testsFailed(0)
{
}

void
hwut::Reporter::initialize()
{
	outputStream << xpcc::accessor::asFlash(tapVersion)
				 << xpcc::accessor::asFlash(tapDate);
}

void
hwut::Reporter::nextTestSuite(xpcc::accessor::Flash<char> name)
{
	testName = name;
	outputStream << xpcc::accessor::asFlash(tapHash)
				 << testName
				 << xpcc::endl;
}

xpcc::IOStream&
hwut::Reporter::reportPass(xpcc::accessor::Flash<char> checkString)
{
	testsPassed++;
	outputStream << xpcc::accessor::asFlash(tapOk)
				 << (testsPassed + testsFailed)
				 << xpcc::accessor::asFlash(tapDash)
				 << checkString;
	return outputStream;
}

xpcc::IOStream&
hwut::Reporter::reportFailure(xpcc::accessor::Flash<char> checkString, unsigned int lineNumber)
{
	testsFailed++;
	outputStream << xpcc::accessor::asFlash(tapNotOk)
				 << (testsPassed + testsFailed)
				 << xpcc::accessor::asFlash(tapDash)
				 << checkString
				 << xpcc::accessor::asFlash(tapFailLine)
				 << lineNumber
				 << xpcc::accessor::asFlash(tapFailMessage);
	return outputStream;
}

void
hwut::Reporter::printSummary()
{
	outputStream << xpcc::accessor::asFlash(tapTests) << (testsFailed + testsPassed) << xpcc::endl;

	if (testsFailed > 0)
	{
		uint_fast16_t percent = (testsFailed * 100) / (testsFailed + testsPassed);

		outputStream << xpcc::accessor::asFlash(reportListFailures)
					 // << list failed cases
					 << xpcc::accessor::asFlash(reportFailed)
					 << testsFailed
					 << '/'
					 << (testsFailed + testsPassed)
					 << xpcc::accessor::asFlash(reportTests)
					 << percent
					 << xpcc::accessor::asFlash(reportPercent)
					 << xpcc::endl;
	}
}
