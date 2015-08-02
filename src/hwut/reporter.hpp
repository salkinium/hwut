/*
 * reporter.hpp
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

#ifndef	HWUT_REPORTER_HPP
#define	HWUT_REPORTER_HPP

#include <stdint.h>

#include <xpcc/io/iostream.hpp>
#include <xpcc/architecture/driver/accessor/flash.hpp>

namespace hwut
{
	/**
	 * \brief	%Reporter
	 *
	 * Used to generate the visible output.
	 *
	 * \author	Fabian Greif
	 * \ingroup	hwut
	 */
	class Reporter
	{
	public:
		/**
		 * \brief	Constructor
		 *
		 * \param	device	IODevice used for printing
		 */
		Reporter(xpcc::IODevice& device);


		/// Prints "TAP version 13\n"
		void
		initialize();

		/**
		 * \brief	Switch to the next test suite
		 *
		 * \param	name	Name of the test suite
		 */
		void
		nextModule(xpcc::accessor::Flash<char> name);

		void
		nextFunction(xpcc::accessor::Flash<char> name);

		/**
		 * \brief	Reported a failed test
		 *
		 * Generates a basic failure message, the returned stream can then
		 * be used to write some more specific information about the failure.
		 */
		xpcc::IOStream&
		report(bool pass, unsigned int lineNumber, xpcc::accessor::Flash<char> type);

		/**
		 * \brief	Writes a summary of all the tests
		 *
		 * Basically the total number of failed and passed tests and then
		 * 'OK' if there was no failure or 'FAIL' otherwise.
		 */
		void
		printSummary();

	private:
		xpcc::IOStream outputStream;
		xpcc::accessor::Flash<char> testModule;
		xpcc::accessor::Flash<char> testFunction;

		uint_fast16_t testsPassed;
		uint_fast16_t testsFailed;
	};
}

#endif	// HWUT_REPORTER_HPP
