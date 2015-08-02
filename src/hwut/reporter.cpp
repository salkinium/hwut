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

namespace report
{
	FLASH_STORAGE_STRING(date) = "\n# HWUT tests (" __DATE__ ", " __TIME__")";
	FLASH_STORAGE_STRING(invalid) = "Invalid";
}

namespace tap
{

	FLASH_STORAGE_STRING(version) = "TAP version 13";
	FLASH_STORAGE_STRING(plan) = "1..";

	FLASH_STORAGE_STRING(descriptionModule) = "\n\t---\n\tmodule: '";
	FLASH_STORAGE_STRING(descriptionFunction) = "'\n\tfunction: '";
	FLASH_STORAGE_STRING(descriptionLine) = "'\n\tline: ";
	FLASH_STORAGE_STRING(descriptionType) = "\n\ttype: '";
	FLASH_STORAGE_STRING(descriptionEnd) = "\n\t...";

	FLASH_STORAGE_STRING(notOk) = "\nnot ok ";
	FLASH_STORAGE_STRING(ok) = "\nok ";

	FLASH_STORAGE_STRING(reportFailed) = "\nFailed ";
	FLASH_STORAGE_STRING(reportTests) = " tests, ";
	FLASH_STORAGE_STRING(reportPercent) = "\% okay";
}

hwut::Reporter::Reporter(xpcc::IODevice& device) :
	outputStream(device),
	testModule(xpcc::accessor::asFlash(report::invalid)),
	testFunction(xpcc::accessor::asFlash(report::invalid)),
	testsPassed(0), testsFailed(0)
{
}

void
hwut::Reporter::initialize()
{
	outputStream << xpcc::accessor::asFlash(tap::version)
				 << xpcc::accessor::asFlash(report::date);
}

void
hwut::Reporter::nextModule(xpcc::accessor::Flash<char> name)
{
	testModule = name;
}

void
hwut::Reporter::nextFunction(xpcc::accessor::Flash<char> name)
{
	testFunction = name;
}

xpcc::IOStream&
hwut::Reporter::report(bool pass, unsigned int lineNumber, xpcc::accessor::Flash<char> type)
{
	if (pass)	testsPassed++;
	else		testsFailed++;

	if (testsPassed + testsFailed > 1) outputStream << xpcc::accessor::asFlash(tap::descriptionEnd);

	outputStream << xpcc::accessor::asFlash(pass ? tap::ok : tap::notOk)
				 << (testsPassed + testsFailed)
				 << xpcc::accessor::asFlash(tap::descriptionModule)
				 << testModule
				 << xpcc::accessor::asFlash(tap::descriptionFunction)
				 << testFunction
				 << xpcc::accessor::asFlash(tap::descriptionLine)
				 << lineNumber
				 << xpcc::accessor::asFlash(tap::descriptionType)
				 << type << '\'';
	return outputStream;
}

void
hwut::Reporter::printSummary()
{
	outputStream << xpcc::accessor::asFlash(tap::descriptionEnd);
	if (testsFailed > 0)
	{
		uint_fast16_t percent = (testsFailed * 100) / (testsFailed + testsPassed);

		outputStream << xpcc::accessor::asFlash(tap::reportFailed)
					 << testsFailed
					 << '/'
					 << (testsFailed + testsPassed)
					 << xpcc::accessor::asFlash(tap::reportTests)
					 << percent
					 << xpcc::accessor::asFlash(tap::reportPercent)
					 << xpcc::endl;
	}

	outputStream << xpcc::accessor::asFlash(tap::plan) << (testsFailed + testsPassed) << xpcc::endl;
}
